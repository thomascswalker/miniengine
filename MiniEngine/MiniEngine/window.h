#ifndef WINDOW_H
#define WINDOW_H
#endif

#include <windows.h>
#include <WinBase.h>
#include <gdiplus.h>

class Window
{
public:
    // Constructor
    Window(HINSTANCE hInstance);

    // Methods
    void show();
    bool shouldClose();
    void pollEvents();
    HWND hwnd();

    // Properties
    MSG _msg = { };

private:
    HWND _hwnd;
};

