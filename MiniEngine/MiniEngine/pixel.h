#ifndef PIXEL_H
#define PIXEL_H

#include <cstddef>
#include <cstdint>

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

#endif