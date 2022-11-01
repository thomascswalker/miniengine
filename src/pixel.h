#ifndef PIXEL_H
#define PIXEL_H

#include "api.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

class Pixel
{
public:
    Pixel(int r, int g, int b) : _r(r), _g(g), _b(b) {};

    int r() { return _r; }
    int g() { return _g; }
    int b() { return _b; }

    int hex();

private:
    int _r;
    int _g;
    int _b;
};

MINI_NAMESPACE_CLOSE

#endif