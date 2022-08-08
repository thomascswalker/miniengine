#include <string>
#include "window.h"

#ifndef MAIN_WINDOW_TIMER_ID
    #define MAIN_WINDOW_TIMER_ID 1001
#endif

#ifndef MAIN_WINDOW_TIMER
    #define MAIN_WINDOW_TIMER 1
#endif

static bool bQuit = false;

int getRefreshRate()
{
    DEVMODE screen;
    memset(&screen, 0, sizeof(DEVMODE));
    int refreshRate = screen.dmDisplayFrequency;
    return refreshRate;
}

LRESULT CALLBACK windowProcessMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    
    auto w = Window::getInstance();

    // Swap the current framebuffer
    auto currentBuffer = w->getCurrentBuffer();
    w->swapFramebuffers();

    // Get current frame size
    UINT width = LOWORD(lParam);
    UINT height = HIWORD(lParam);

    // Switch on message typew
    switch (uMsg)
    {
        case WM_CREATE:
        {
            SetTimer(hwnd, MAIN_WINDOW_TIMER_ID, MAIN_WINDOW_TIMER, NULL);
            return 0;
        }

        case WM_QUIT:
        case WM_DESTROY:
        {
            bQuit = true;
            return 0;
        }

        case WM_ERASEBKGND:
        {
            return TRUE;
        }

        case WM_TIMER:
        {
            InvalidateRect(hwnd, NULL, FALSE);
            UpdateWindow(hwnd);
        }

        case WM_SIZE:
        {
            // Resize the framebuffer
            currentBuffer.setSize(width, height);
        }

        case WM_PAINT:
        {
            // Get the current framebuffer's bitmap
            currentBuffer.createFrameBitmap();
            auto newBitmap = currentBuffer.getFrameBitmap();

            // Create our source and target device contexts
            auto targetHdc = GetDC(hwnd);
            HDC sourceDc = CreateCompatibleDC(targetHdc);

            // Select our new bitmap object
            HBITMAP oldbitmap = (HBITMAP) SelectObject(sourceDc, newBitmap);
            GetObject(newBitmap, sizeof(newBitmap), &newBitmap);

            // Copy from source context to target context
            BitBlt(
                targetHdc,      // Target device context
                0, 0,           // Target position
                width, height,  // Target width + height
                sourceDc,       // Source device context
                0, 0,           // Source position
                SRCCOPY
            );

            // Delete the old bitmap object
            SelectObject(sourceDc, oldbitmap);
            DeleteDC(sourceDc);

            // At the end of painting, swap buffers
            break;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ PWSTR pCmdLine,
    _In_ int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = windowProcessMessage;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.
    auto hwnd = CreateWindowEx(
        0,                                // Optional window styles.
        CLASS_NAME,                       // Window class
        L"MiniEngine",                    // Window text
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, // Window style

        0,      // X position
        0,      // Y position
        100,    // Width
        100,    // Height

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    // Show the window
    ShowWindow(hwnd, 1);

    // Run the message loop.
    while (!bQuit)
    {
        static MSG message = { 0 };
        while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&message);
        }
    };

    return 0;
}
