#include "core.h"

MINI_NAMESPACE_OPEN

int getRefreshRate()
{
    DEVMODE screen;
    memset(&screen, 0, sizeof(DEVMODE));
    int refreshRate = screen.dmDisplayFrequency;
    return refreshRate;
}

Size getScreenSize()
{
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();

    GetWindowRect(hDesktop, &desktop);

    return Size(desktop.right, desktop.bottom);
}

Rect::Rect(Vector2 min, Vector2 max)
{
    m_x = min.x();
    m_y = min.y();
    m_width = max.x() - min.x();
    m_height = max.y() - min.y();
}

double getCurrentTime()
{            
    auto duration = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    return (double) duration.count();
}

void print(const char* format, ...)
{
    char text[1024];
    va_list arg;
    va_start(arg, format);
    _vsnprintf_s(text, sizeof(text), format, arg);
    va_end(arg);
    
    unsigned long long size = strlen(text) + 1;
    wchar_t* wtext = new wchar_t[size];
    
    size_t outSize;
    mbstowcs_s(&outSize, wtext, size, text, size-1);
    OutputDebugString(wtext);
}

MINI_NAMESPACE_CLOSE