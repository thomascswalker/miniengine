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

std::vector<unsigned int> Framebuffer::getMemoryBuffer()
{
    auto ptr  = (unsigned int*)m_memoryBuffer;  // Pointer to buffer
    auto size = getBufferSize();                // Size of buffer (pixel count)

    std::vector<unsigned int> buffer;           // Create a new vector
    buffer.reserve(size);                       // Reserve size for this vector to be
                                                // the size of the pixel count

    for (int i = 0; i < size; i++)              // Iterate through the memory buffer
    {
        auto pixel = *ptr;                      // Get the value at the current pointer
        buffer.push_back(pixel);                // Add to the vector
        (*ptr)++;                               // Increment the pointer
    }

    return buffer;                              // Return our vector buffer
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

void Framebuffer::renderGradient()
{
    uint8 *row = (uint8*)m_memoryBuffer;

    for (int y = 0; y < m_height; y++)
    {
        uint32 *pixel = (uint32*)row;
        for (int x = 0; x < m_width; x++)
        {
            uint8 red = x;
            uint8 green = y;
            uint8 blue = 0;

            *pixel++ = (red << 16) | (green << 8) | blue;
        }

        row += m_rowLength;
    }
}

void Framebuffer::fillRect(int x0, int y0, int x1, int y1, MColor color)
{
    x0 = MCore::clamp(&x0, 0, m_width);
    x1 = MCore::clamp(&x1, 0, m_width);
    
    y0 = MCore::clamp(&y0, 0, m_height);
    y1 = MCore::clamp(&y1, 0, m_height);

    // For each pixel...
    for (int y = y0; y < y1; y++)
    {
        // Get the first pointer position in our memory buffer
        uint32 *pixel = (uint32*)m_memoryBuffer;
        pixel += (uint32)(x0 + (y * m_width));
        for (int x = x0; x < x1; x++)
        {
            *pixel++ = color.hex();      // Set the color at this position in memory
        }
    }
}

bool operator == (const Framebuffer& f1, const Framebuffer& f2)
{
    return typeid(f1) == typeid(f2);
}