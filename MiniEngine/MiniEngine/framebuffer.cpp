#include "framebuffer.h"

using namespace Core;

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
    if (m_colorBuffer)
    {
        VirtualFree(m_colorBuffer, 0, MEM_RELEASE);
        m_colorBuffer = nullptr;
    }

    // Calculate the new buffer size and allocate memory of that size
    int bufferSize = m_width * m_height * m_bytesPerPixel;

    // Allocate the memory buffer
    m_colorBuffer = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    // Update the buffer BitmapInfo struct with the updated buffer size,
    // and width and height
    m_bufferBmi.bmiHeader.biSize = sizeof(m_bufferBmi.bmiHeader);
    m_bufferBmi.bmiHeader.biWidth = m_width;
    m_bufferBmi.bmiHeader.biHeight = -m_height; // When height is negative, it will invert the bitmap vertically.

    // Set the row length to the current window width * bytes per pixel (4)
    m_rowLength = m_width * m_bytesPerPixel;
}

void Framebuffer::setSize(Core::Size size)
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

void Framebuffer::setVertexBufferData(std::vector<Vertex> data)
{
    m_vertices = data;
}

void Framebuffer::setIndexBufferData(std::vector<int> data)
{
    m_indices = data;
}

int Framebuffer::getNumVertices()
{
    return m_numVertices;
}

int Framebuffer::getNumIndices()
{
    return m_numIndices;
}

Vector2 Framebuffer::worldToScreen(Vector3 vector, Matrices::Matrix4 matrix)
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

    // Clip screen coordinates to screen width/height
    screenCoord.setX(m_width  < screenCoord.x() ? m_width  : screenCoord.x());
    screenCoord.setY(m_height < screenCoord.y() ? m_height : screenCoord.y());

    return screenCoord;
}

void Framebuffer::clear()
{
    drawRect(0, 0, m_width, m_height, Color(0, 0, 0));
}

void Framebuffer::setPixel(int x, int y, Color color)
{
    uint32* pixelPtr = (uint32*)m_colorBuffer;
    uint32 offset = x + (y * m_width);
    pixelPtr += offset;
    *pixelPtr = color.hex();
}

void Framebuffer::drawGradient()
{
    // The start position of the current row
    uint8 *rowPtr = (uint8*)m_colorBuffer;

    for (int y = 0; y < m_height; y++)
    {
        // Initial pointer position in our memory buffer
        uint32 *pixel = (uint32*)rowPtr;
        for (int x = 0; x < m_width; x++)
        {
            uint8 red = x;      // Red channel gradient getting brighter left -> right
            uint8 green = y;    // Green channel gradient getting brighter top -> down
            uint8 blue = 0;     // No blue
            auto color = Color(red, green, blue);

            // Set the color at this position in memory
            *pixel++ = color.hex();
        }

        rowPtr += m_rowLength;
    }
}

void Framebuffer::drawRect(int x0, int y0, int x1, int y1, Color color)
{
    x0 = Math::clamp(&x0, 0, m_width);
    x1 = Math::clamp(&x1, 0, m_width);
    
    y0 = Math::clamp(&y0, 0, m_height);
    y1 = Math::clamp(&y1, 0, m_height);

    // For each pixel...
    for (int y = y0; y < y1; y++)
    {
        for (int x = x0; x < x1; x++)
        {
            // Set the color at this position in memory
            setPixel(x, y, color);
        }
    }
}

void Framebuffer::drawCircle(int cx, int cy, int r, Color color)
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
        for (int x = x0; x < x1; x++)
        {
            auto dx = x - cx;
            auto dy = y - cy;
            if (pow(dx, 2) + pow(dy, 2) <= rsqr)
            {
                setPixel(x, y, color);
            }
        }
    }
}

void Framebuffer::drawTri(Vector2& v1, Vector2& v2, Vector2& v3, Color color)
{
    // Determine the min/max threshold for drawing
    std::vector<float> xList = {v1.x(), v2.x(), v3.x()};
    std::vector<float> yList = {v1.y(), v2.y(), v3.y()};

    auto minX = *std::min_element(std::begin(xList), std::end(xList));
    auto maxX = *std::max_element(std::begin(xList), std::end(xList));
    auto minY = *std::min_element(std::begin(yList), std::end(yList));
    auto maxY = *std::max_element(std::begin(yList), std::end(yList));

    for (int y = minY; y < maxY; y++)                       // Bottom to top
    {
        for (int x = minX; x < maxX; x++)                   // Left to right
        {
            Vector2 point(x,y);                             // Point at current x, y
            if (Math::isPointInTriangle(point, v1, v2, v3)) // Is this point in our triangle?
            {
                setPixel(x, y, color);                     // If it is, colour here
            }
        }
    }
}

/*
https://en.wikipedia.org/wiki/Line_drawing_algorithm
*/
void Framebuffer::drawLine(Vector2& v1, Vector2& v2, Color color)
{
    std::vector<float> xList = { v1.x(), v2.x() };
    std::vector<float> yList = { v1.y(), v2.y() };

    auto minX = *std::min_element(std::begin(xList), std::end(xList));
    auto maxX = *std::max_element(std::begin(xList), std::end(xList));
    auto minY = *std::min_element(std::begin(yList), std::end(yList));
    auto maxY = *std::max_element(std::begin(yList), std::end(yList));

    float step;

    float dx = maxX - minX;
    float dy = maxY - minY;

    auto adx = abs(dx);
    auto ady = abs(dy);
    step = (adx >= ady) ? adx : ady;

    dx = dx / step;
    dy = dy / step;

    float x = minX;
    float y = minY;
    int i = 1;

    while (i <= step)
    {
        setPixel(x, y, color);

        x += dx;
        y += dy;
        i++;
    }
}

void Framebuffer::drawScene(Matrices::Matrix4 m, bool bDrawFaces, bool bDrawEdges, bool bDrawVertices)
{
    for (int i = 0; i < m_indices.size(); i += 3)
    {
        // Start indices index
        int index = i;

        // Get vertex indices
        int idx1 = m_indices[i];
        int idx2 = m_indices[i + 1];
        int idx3 = m_indices[i + 2];

        // Get vertexes from indices
        Vertex vtx1 = m_vertices[idx1];
        Vertex vtx2 = m_vertices[idx2];
        Vertex vtx3 = m_vertices[idx3];
        
        // Convert world pos to screen pos for each vertex
        Vector2 vtx1s = worldToScreen(vtx1.pos(), m);
        Vector2 vtx2s = worldToScreen(vtx2.pos(), m);
        Vector2 vtx3s = worldToScreen(vtx3.pos(), m);

        // Draw each face
        if (bDrawFaces)
        {
            drawTri(vtx1s, vtx2s, vtx3s, Color::white());
        }

        // Draw each edge
        if (bDrawEdges)
        {
            drawLine(vtx1s, vtx2s, Color::green());
            drawLine(vtx1s, vtx3s, Color::green());
            drawLine(vtx2s, vtx3s, Color::green());
        }

        // Draw each vertex
        if (bDrawVertices)
        {
            drawCircle(vtx1s.x(), vtx1s.y(), 3, Color::blue());
            drawCircle(vtx2s.x(), vtx1s.y(), 3, Color::blue());
            drawCircle(vtx3s.x(), vtx1s.y(), 3, Color::blue());
        }
    }
}

bool operator == (const Framebuffer& f1, const Framebuffer& f2)
{
    return typeid(f1) == typeid(f2);
}

