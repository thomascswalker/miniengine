#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#endif

#include <memory>
#include <typeinfo>
#include <windows.h>
#include <gdiplus.h>
#include <cassert>

#include "core.h"
#include "color.h"
#include "matrix.h"

class Framebuffer
{
public:
    Framebuffer(HWND hwnd);
    ~Framebuffer();

    // Memory
    int getWidth() { return m_width; }
    void setWidth(int width) { m_width = width; }
    int getHeight() { return m_height; }
    void setHeight(int height) { m_height = height; }
    void setSize(MCore::MSize size);
    void setSize(int width, int height);

    HWND getHwnd() { return m_hwnd; }
    void allocate();
    BITMAPINFO* getBitmapInfo() { return &m_bufferBmi; }
    void* getMemoryPtr() { return (unsigned int*)m_memoryBuffer; }
    int getBufferSize() { return m_width * m_height * sizeof(unsigned int); }

    // Math
    Vector2 worldToScreen(Vector3 vector, Matrix4 matrix);

    // Drawing
    void clear();
    void fillRect(int x0, int y0, int x1, int y1, MColor color);
    void renderGradient();



protected:
    friend bool operator == (const Framebuffer&,
                             const Framebuffer&);

private:
    // Window handle
    const HWND m_hwnd = HWND();

    int m_width = 640;
    int m_height = 480;

    SIZE_T m_bufferSize;
    void* m_memoryBuffer;
    BITMAPINFO m_bufferBmi;

    // Number of bytes per pixel
    const int m_bytesPerPixel = 4;

    // Length of row in bytes (width * bytes per pixel)
    int m_rowLength = 0;
};
