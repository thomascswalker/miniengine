#ifndef APPLICATION_H
#define APPLICATION_H

//#include "core.h"
#include <Windows.h>
#include <windowsx.h>

class Application
{
protected:
    static Application *instance;
    Application();

public:
    static Application* getAppInstance();

    void init();
    int run();

    HWND getHwnd() {return m_hwnd;}
    void setHwnd(HWND hwnd) {m_hwnd = hwnd;}

    HINSTANCE getHInstance() {return m_hInstance;}
    void setHInstance(HINSTANCE hInstance) {m_hInstance = hInstance;}

private:
    HWND m_hwnd = 0;
    HINSTANCE m_hInstance = 0;
};

#endif