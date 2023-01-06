#include "core.h"

namespace Graphics
{
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

    double getCurrentTime()
    {
        auto duration = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        return (double)duration.count();
    }

    int getDeltaTime(int previous, int offset)
    {
        return (std::clock() - previous) + offset;
    }

    void print(const char* format, ...)
    {
        char text[1024];
        va_list arg;
        va_start(arg, format);
        _vsnprintf_s(text, sizeof(text), format, arg);
        va_end(arg);

        unsigned long long size = strlen(text) + 1;
        auto* wtext = new wchar_t[size];

        size_t outSize;
        mbstowcs_s(&outSize, wtext, size, text, size - 1);
        OutputDebugString(reinterpret_cast<LPCSTR>(wtext));
    }
}