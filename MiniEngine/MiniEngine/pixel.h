#ifndef PIXEL_H
#define PIXEL_H
#endif

class Pixel
{
public:
    Pixel(float r, float g, float b) : _r(r), _g(g), _b(b) {};

    float r() { return _r; }
    float g() { return _g; }
    float b() { return _b; }

private:
    float _r;
    float _g;
    float _b;
};