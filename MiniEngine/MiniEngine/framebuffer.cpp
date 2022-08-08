#include "framebuffer.h"

Framebuffer::Framebuffer()
{
    
}

Framebuffer::~Framebuffer()
{

}

HBITMAP Framebuffer::getFrameBitmap()
{
    return frameBitmap;
}

void Framebuffer::createFrameBitmap()
{
    int bufferSize = _width * _height * 4;
    _pixelBuffer = std::vector<unsigned char> (bufferSize, unsigned char(0));

    for (int i = 0; i < bufferSize; i += 4)
    {
        _pixelBuffer[i]     = unsigned char(0);     // Blue
        _pixelBuffer[i + 1] = unsigned char(150);   // Green
        _pixelBuffer[i + 2] = _redColor;            // Red
        _pixelBuffer[i + 3] = unsigned char(0);     // Alpha
    }

    /*
    char buffer[100];
    sprintf_s(buffer, "Pixel count: %i\n", _pixelBuffer.size());
    OutputDebugStringA(buffer);
    */

    frameBitmap = CreateBitmap(_width, _height, 1, 32, _pixelBuffer.data());
    _pixelBuffer.clear();
}

void Framebuffer::setWidth(int width)
{
    _width = width;
}

void Framebuffer::setHeight(int height)
{
    _height = height;
}

void Framebuffer::setSize(int width, int height)
{
    setWidth(width);
    setHeight(height);
}

bool operator == (const Framebuffer& f1, const Framebuffer& f2)
{
    return typeid(f1) == typeid(f2);
}