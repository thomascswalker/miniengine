#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#endif

#include <vector>
#include <memory>
#include <typeinfo>
#include "pixel.h"

class Framebuffer
{
public:
    Framebuffer();
    ~Framebuffer();

    float _width = 100;
    float _height = 100;
    std::vector<Pixel> pixels;

protected:
    friend bool operator == (const Framebuffer&,
                             const Framebuffer&);
};
