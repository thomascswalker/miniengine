#include "window.h"

Window::Window()
{
    // Create the framebuffer
    frontBuffer = new Framebuffer();
    frontBuffer->_redColor = 255;
    backBuffer = new Framebuffer();
    backBuffer->_redColor = 50;

    currentBuffer = frontBuffer;
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
    if (currentBuffer != frontBuffer)
    {
        currentBuffer = frontBuffer;
    }
    else
    {
        currentBuffer = backBuffer;
    }
}

Framebuffer Window::getCurrentBuffer()
{
    return *currentBuffer;
}

Window *Window::instance = 0;