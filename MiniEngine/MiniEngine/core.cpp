#include "core.h"

int MCore::clamp(int *value, int min, int max)
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

int MCore::getRefreshRate()
{
    DEVMODE screen;
    memset(&screen, 0, sizeof(DEVMODE));
    int refreshRate = screen.dmDisplayFrequency;
    return refreshRate;
}

MCore::MSize MCore::getScreenSize()
{
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();

    GetWindowRect(hDesktop, &desktop);

    return MCore::MSize(desktop.right, desktop.bottom);
}