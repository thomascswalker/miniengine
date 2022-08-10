#ifndef CORE_H
#define CORE_H

#ifndef MAIN_WINDOW_TIMER_ID
    #define MAIN_WINDOW_TIMER_ID 1001
#endif

#include <windows.h>
#include <windowsx.h>

class MSize
{
public:
    MSize(int width, int height) : _width(width), _height(height) {};

    int width() {return _width;}
    int height() {return _height;}
    int setWidth(int width) {_width = width;}
    int setHeight(int height) {_height = height;}

private:
    int _width;
    int _height;
};

int clamp(int *value, int min, int max)
{
    if (*value < min)
    {
        return min;
    }
    if (*value > max)
    {
        return max;
    }
    return *value;
}

int getRefreshRate()
{
    DEVMODE screen;
    memset(&screen, 0, sizeof(DEVMODE));
    int refreshRate = screen.dmDisplayFrequency;
    return refreshRate;
}

MSize getScreenSize()
{
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();

    GetWindowRect(hDesktop, &desktop);

    return MSize(desktop.right, desktop.bottom);
}

#endif
