#include "window.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    bool bRet;
    auto w = Window(hInstance);

    if (w.hwnd() == NULL)
    {
        return 0;
    }

    w.show();

    // Run the message loop.
    while (!w.shouldClose())
    {
        w.pollEvents();
    };

    //Gdiplus::GdiplusShutdown(token);

    return 0;
}
