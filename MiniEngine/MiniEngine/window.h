#ifndef WINDOW_H
#define WINDOW_H
#endif

#include <windows.h>
#include <WinBase.h>
#include <gdiplus.h>
#include <random>
#include <cstdlib>
#include <ctime>
#include <WinUser.h>

#include "framebuffer.h"

class Window
{
public:
    // Constructor
    Window(HINSTANCE hInstance);

    // Methods
    void show();
    bool shouldClose();
    void pollEvents();
    void swapFramebuffers();
    //void redraw();
    HWND hwnd();

private:
    // Properties
    MSG _msg = { };
    HWND _hwnd;

    Framebuffer currentBuffer;
    Framebuffer frontBuffer;
    Framebuffer backBuffer;
};

