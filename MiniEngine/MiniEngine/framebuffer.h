#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#endif

#include <vector>
#include <memory>
#include <typeinfo>
#include <windows.h>
#include <gdiplus.h>

class Framebuffer
{
public:
    Framebuffer();
    ~Framebuffer();

    HBITMAP getFrameBitmap();
    void createFrameBitmap();

    int getWidth() {return _width;}
    int getHeight() {return _height;}
    void setWidth(int width);
    void setHeight(int height);
    void setSize(int width, int height);

    unsigned char _redColor;

protected:
    friend bool operator == (const Framebuffer&,
                             const Framebuffer&);

private:
    HBITMAP frameBitmap;
    std::vector<BYTE> _pixelBuffer;
    int _width = 500;
    int _height = 500;
    
};
