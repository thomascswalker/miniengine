#include "window.h"

#pragma comment (lib, "Gdiplus.lib")

void Paint(HDC hdc, int width, int height)
{
    Gdiplus::Graphics graphics(hdc);

    // Create an image.
    Gdiplus::Image image(L"C:/Users/Tom/Pictures/background.png");

    // Resize the image
    Gdiplus::Rect size(0, 0, width, height);

    // Draw the original source image.
    graphics.DrawImage(&image, size);
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT clientArea;
        GetClientRect(hwnd, &clientArea);
        int width = clientArea.right - clientArea.left;
        int height = clientArea.bottom - clientArea.top;

        // All painting occurs here, between BeginPaint and EndPaint.
        HBRUSH hBrush = CreateSolidBrush(RGB(0,0,0));
        FillRect(hdc, &clientArea, hBrush);

        Paint(hdc, width, height);

        EndPaint(hwnd, &ps);
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

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    _hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"MiniEngine",                  // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );
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

HWND Window::hwnd()
{
    return _hwnd;
}
