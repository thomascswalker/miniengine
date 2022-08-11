#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#endif

#include <vector>
#include <memory>
#include <typeinfo>
#include <windows.h>
#include <gdiplus.h>
#include <cassert>

#include "core.h"
#include "color.h"

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
    void* getMemoryBuffer() { return m_memoryBuffer; }

    // Drawing
    void fillRect(int x0, int y0, int x1, int y1, MColor color);

    SIZE_T m_bufferSize;
    void* m_memoryBuffer;

protected:
    friend bool operator == (const Framebuffer&,
                             const Framebuffer&);

private:
    const HWND m_hwnd = HWND();

    int m_width = 640;
    int m_height = 480;
    
    BITMAPINFO m_bufferBmi;
};
