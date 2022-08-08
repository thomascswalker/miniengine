#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include <gdiplus.h>

#include "framebuffer.h"

class Window
{
protected:
    static Window *instance;
    Window();

public:
    // Methods
    static Window* getInstance();
    void swapFramebuffers();
    Framebuffer getCurrentBuffer();

private:
    // Properties
    char* currentBuffer;
    Framebuffer frontBuffer;
    Framebuffer backBuffer;
};

#endif