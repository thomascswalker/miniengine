#include "framebuffer.h"

Framebuffer::Framebuffer()
{
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            Pixel p(128, 128, 128);
            pixels.push_back(p);
        }
    }
};

Framebuffer::~Framebuffer()
{
}

bool operator == (const Framebuffer& f1, const Framebuffer& f2)
{
    return typeid(f1) == typeid(f2);
}