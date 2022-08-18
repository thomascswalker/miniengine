#include "framebuffer.h"

using namespace MCore;

Framebuffer::Framebuffer(HWND hwnd)
    : m_hwnd(hwnd)
{
    // Set BitmapInfo struct default values
    m_bufferBmi.bmiHeader.biPlanes = 1;
    m_bufferBmi.bmiHeader.biBitCount = 32;
    m_bufferBmi.bmiHeader.biCompression = BI_RGB;

    // Do an initial allocation of the framebuffer
    allocate();
}

Framebuffer::~Framebuffer()
{
    
}

void Framebuffer::allocate()
{
    // Clear the memory buffer
    if (m_memoryBuffer)
    {
        VirtualFree(m_memoryBuffer, 0, MEM_RELEASE);
        m_memoryBuffer = nullptr;
    }

    // Calculate the new buffer size and allocate memory of that size
    int bufferSize = m_width * m_height * m_bytesPerPixel;

    // Allocate the memory buffer
    m_memoryBuffer = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    // Update the buffer BitmapInfo struct with the updated buffer size,
    // and width and height
    m_bufferBmi.bmiHeader.biSize = sizeof(m_bufferBmi.bmiHeader);
    m_bufferBmi.bmiHeader.biWidth = m_width;
    m_bufferBmi.bmiHeader.biHeight = -m_height; // When height is negative, it will invert the bitmap vertically.

    // Set the row length to the current window width * bytes per pixel (4)
    m_rowLength = m_width * m_bytesPerPixel;
}

void Framebuffer::setSize(MCore::MSize size)
{
    m_width = size.width();
    m_height = size.height();
    allocate();
}

void Framebuffer::setSize(int width, int height)
{
    m_width = width;
    m_height = height;
    allocate();
}

Vector2 Framebuffer::worldToScreen(Vector3 vector, Matrix4 matrix)
{
    Vector4 clipCoords;
    clipCoords.setX(vector.x() * matrix[0] +
                    vector.y() * matrix[4] +
                    vector.z() * matrix[8] +
                    matrix[12]);
    clipCoords.setY(vector.x() * matrix[1] +
                    vector.y() * matrix[5] +
                    vector.z() * matrix[9] +
                    matrix[13]);
    clipCoords.setZ(vector.x() * matrix[2] +
                    vector.y() * matrix[6] +
                    vector.z() * matrix[10] +
                    matrix[14]);
    clipCoords.setW(vector.x() * matrix[3] +
                    vector.y() * matrix[7] +
                    vector.z() * matrix[11] +
                    matrix[15]);

    if (clipCoords.w() < 0.1f)
    {
        return Vector2();
    }

    Vector3 ndc; // Normalized device coordinates
    ndc.setX(clipCoords.x() / clipCoords.w());
    ndc.setY(clipCoords.y() / clipCoords.w());
    ndc.setZ(clipCoords.z() / clipCoords.w());

    // Transform to screen coordinates
    Vector2 screenCoord;
    screenCoord.setX((m_width / 2 * ndc.x()) + (ndc.x() + m_width / 2));
    screenCoord.setY((m_height / 2 * ndc.y()) + (ndc.y() + m_height / 2));

    return screenCoord;
}

void Framebuffer::clear()
{
    drawRect(0, 0, m_width, m_height, MColor(0, 0, 0));
}

void Framebuffer::drawGradient()
{
    // The start position of the current row
    uint8 *rowPtr = (uint8*)m_memoryBuffer;

    for (int y = 0; y < m_height; y++)
    {
        // Initial pointer position in our memory buffer
        uint32 *pixel = (uint32*)rowPtr;
        for (int x = 0; x < m_width; x++)
        {
            uint8 red = x;      // Red channel gradient getting brighter left -> right
            uint8 green = y;    // Green channel gradient getting brighter top -> down
            uint8 blue = 0;     // No blue
            auto color = MColor(red, green, blue);

            // Set the color at this position in memory
            *pixel++ = color.hex();
        }

        rowPtr += m_rowLength;
    }
}

void Framebuffer::drawRect(int x0, int y0, int x1, int y1, MColor color)
{
    x0 = Math::clamp(&x0, 0, m_width);
    x1 = Math::clamp(&x1, 0, m_width);
    
    y0 = Math::clamp(&y0, 0, m_height);
    y1 = Math::clamp(&y1, 0, m_height);

    // For each pixel...
    for (int y = y0; y < y1; y++)
    {
        // Initial pointer position in our memory buffer
        uint32 *pixel = (uint32*)m_memoryBuffer;

        // Offset across the buffer
        uint32 offset = x0 + (y * m_width);
        pixel += offset;

        for (int x = x0; x < x1; x++)
        {
            // Set the color at this position in memory
            *pixel++ = color.hex();
        }
    }
}

void Framebuffer::drawCircle(int cx, int cy, int r, MColor color)
{
    // Top left
    int x0 = cx - r;
    int y0 = cy - r;

    // Bottom right
    int x1 = cx + r;
    int y1 = cy + r;

    // Clamp based on buffer width/height
    x0 = Math::clamp(&x0, 0, m_width);
    y0 = Math::clamp(&y0, 0, m_height);

    x1 = Math::clamp(&x1, 0, m_width);
    y1 = Math::clamp(&y1, 0, m_height);

    auto rsqr = pow(r, 2);

    for (int y = y0; y < y1; y++)
    {
        // Initial pointer position in our memory buffer
        uint32 *pixel = (uint32*)m_memoryBuffer;
        uint32 offset = x0 + (y * m_width);
        pixel += offset;

        for (int x = x0; x < x1; x++)
        {
            auto dx = x - cx;
            auto dy = y - cy;

            if (pow(dx, 2) + pow(dy, 2) <= rsqr)
            {
                *pixel++ = color.hex();
            }
        }
    }
}

// https://austinmorlan.com/posts/drawing_a_triangle/
//
//  v3 (blue)        v2 (green)
//
//
//
//
//           v1 (red)
//
void Framebuffer::drawTri(Vector2& v1, Vector2& v2, Vector2& v3, MColor color)
{
    for (int y = v3.y(); y < v1.y(); y++)               // Bottom to top
    {
        uint32* pixel = (uint32*)m_memoryBuffer;        // Initial memory starting point
        int yOffset = y * m_width;                      // Number of pixels in an entire row
        int xOffset = v3.x();                           // Number of pixels to hit the left-most edge
        pixel += xOffset + yOffset;                     // Linear offset across the entire pixel array

        for (int x = v3.x(); x < v2.x(); x++)           // Left to right
        {
            auto p = Vector2(x, y);                     // Point at current x, y
            if (Math::isPointInTriangle(p, v1, v2, v3)) // Is this point in our triangle?
            {
                *pixel++ = color.hex();                 // If it is, colour here
            }
        }
    }
}

bool operator == (const Framebuffer& f1, const Framebuffer& f2)
{
    return typeid(f1) == typeid(f2);
}

