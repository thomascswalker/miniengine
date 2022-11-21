#ifndef APPLICATION_H
#define APPLICATION_H

#include <Windows.h>
#include <windowsx.h>
#include <string>

#include "framebuffer.h"
#include "meshloader.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

class Application
{
    HWND m_hwnd = 0;
    HINSTANCE m_hInstance = 0;

    Framebuffer* m_buffer {};

    int m_width = 0;
    int m_height = 0;

    Point<int> m_mousePos;
    Point<int> m_mouseLastPos;
    Point<int> m_mouseClickPos;

    double m_currentTime = 0.0;
    double m_deltaTime = 0.0;

    Mesh* m_mesh = new Mesh();

protected:
    static Application *instance;
    Application() {};

    void displayPrintText();
    void displayFps();

public:
    static Application* getAppInstance();

    void init();
    int run();

    HWND getHwnd() { return m_hwnd; }
    void setHwnd(HWND hwnd) {m_hwnd = hwnd; }

    HINSTANCE getHInstance() { return m_hInstance; }
    void setHInstance(HINSTANCE hInstance) { m_hInstance = hInstance; }

    bool loadModel();

    Framebuffer* getFramebuffer() { return m_buffer; }
    void setSize(int width, int height);

    Point<int> getMousePos() { return m_mousePos; }
    Point<int> getMouseLastPos() { return m_mouseLastPos; }
    Point<int> getMouseClickPos() { return m_mouseClickPos; }

    inline void setMousePos(int _x, int _y) { m_mousePos.x = _x; m_mousePos.y = _y; }
    inline void setMouseLastPos(int _x, int _y) { m_mouseLastPos.x = _x; m_mouseLastPos.y = _y; }
    inline void setMouseClickPos(int _x, int _y) { m_mouseClickPos.x = _x; m_mouseClickPos.y = _y; }

    inline void setMousePos(Point<int> p) { m_mousePos = p; }
    inline void setMouseLastPos(Point<int> p) { m_mouseLastPos = p; }
    inline void setMouseClickPos(Point<int> p) { m_mouseClickPos = p; }
};

MINI_NAMESPACE_CLOSE

#endif