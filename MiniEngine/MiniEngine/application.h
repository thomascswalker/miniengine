#ifndef APPLICATION_H
#define APPLICATION_H

#include <Windows.h>
#include <windowsx.h>

#include "framebuffer.h"
#include "core.h"

class Application
{
protected:
    static Application *instance;
    Application() {};

public:
    static Application* getAppInstance();

    void init();
    int run();

    HWND getHwnd() {return m_hwnd;}
    void setHwnd(HWND hwnd) {m_hwnd = hwnd;}

    HINSTANCE getHInstance() {return m_hInstance;}
    void setHInstance(HINSTANCE hInstance) {m_hInstance = hInstance;}

    Framebuffer* getFramebuffer() { return m_buffer; }
    void setSize(int width, int height);
    void setMousePos(int x, int y);

private:
    HWND m_hwnd = 0;
    HINSTANCE m_hInstance = 0;

    Framebuffer* m_buffer {};

    int m_width = 0;
    int m_height = 0;

    int m_mouseX = 0;
    int m_mouseY = 0;


};

#endif