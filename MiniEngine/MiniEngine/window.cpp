#include "window.h"

Window::Window()
{
    // Create the framebuffer
    frontBuffer = Framebuffer();
    frontBuffer._redColor = 255;
    backBuffer = Framebuffer();
    backBuffer._redColor = 150;
}

Window* Window::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Window();
    }
    return instance;
}

void Window::swapFramebuffers()
{
    //if (currentBuffer != &frontBuffer)
    //{
    //    currentBuffer = &frontBuffer;
    //}
    //else
    //{
    //    currentBuffer = &backBuffer;
    //}
}

Framebuffer Window::getCurrentBuffer()
{
    return frontBuffer;
}

Window *Window::instance = 0;