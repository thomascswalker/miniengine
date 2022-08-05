#include "window.h"

#pragma comment (lib, "Gdiplus.lib")

VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
     static BOOL fFlipFlop = FALSE;
     HBRUSH hBrush;
     HDC hdc;
     RECT rc;
     
     fFlipFlop = !fFlipFlop;
     
     GetClientRect(hwnd, &rc);
     
     hdc = GetDC(hwnd);
     hBrush = CreateSolidBrush(fFlipFlop ? RGB(30,30,30) : RGB(68,68,68));
     
     FillRect (hdc, &rc, hBrush);
     ReleaseDC (hwnd, hdc);
     DeleteObject (hBrush);
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Get the refresh rate of the monitor
    DEVMODE screen;
    memset(&screen, 0, sizeof(DEVMODE));
    int refreshRate = screen.dmDisplayFrequency;

    // Switch on message type
    switch (uMsg)
    {
        case WM_CREATE:
            SetTimer(hwnd, 1, refreshRate, TimerProc);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}




Window::Window(HINSTANCE hInstance)
{
    ULONG_PTR token;
    Gdiplus::GdiplusStartupInput input = {0};
    input.GdiplusVersion = 1;
    Gdiplus::GdiplusStartup(&token, &input, NULL);

    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = windowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.
    _hwnd = CreateWindowEx(
        0,                                // Optional window styles.
        CLASS_NAME,                       // Window class
        L"MiniEngine",                    // Window text
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, // Window style

        // Size and position
        640,
        480,
        CW_USEDEFAULT,
        CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    // Create the framebuffer
    frontBuffer = Framebuffer();
    backBuffer = Framebuffer();
}

void Window::show()
{
    ShowWindow(_hwnd, 1);
}

bool Window::shouldClose()
{
    return GetMessage(&_msg, NULL, 0, 0) == 0;
}

void Window::pollEvents()
{
    TranslateMessage(&_msg);
    DispatchMessage(&_msg);
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

//void Window::redraw()
//{
//    RedrawWindow(_hwnd, 0, 0, RDW_INVALIDATE | RDW_UPDATENOW);
//    UpdateWindow(_hwnd);
//}

HWND Window::hwnd()
{
    return _hwnd;
}
