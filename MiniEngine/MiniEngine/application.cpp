#include "application.h"

#ifndef MAIN_WINDOW_TIMER_ID
    #define MAIN_WINDOW_TIMER_ID 1001
#endif

static bool bQuit = false;
void* bufferMemory;
int bufferWidth;
int bufferHeight;
int mouseX;
int mouseY;
BITMAPINFO bufferBmi;

int xOffset;

float frameRate = 1000.0f / 60.0f;

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


void fillColor(int color)
{
    auto pixelPtr = (unsigned int*) bufferMemory;
    for (int y = 0; y < bufferHeight; y++)
    {
        for (int x = 0; x < bufferWidth; x++)
        {
            *pixelPtr = color;
            (pixelPtr)++;
        }
    }
}

void drawRect(int xPos, int yPos, int w, int h, int color)
{
    clamp(&xPos, 0, bufferWidth);
    clamp(&yPos, 0, bufferHeight);

    if (xPos + w > bufferWidth)
    {
        w = bufferWidth - xPos;
    }
    if (yPos + h > bufferHeight)
    {
        h = bufferHeight - yPos;
    }

    for (int y = yPos; y < yPos + h; y++)
    {
        auto pixelPtr = (unsigned int*) bufferMemory + xPos + (y * bufferWidth);
        for (int x = xPos; x < xPos + w; x++)
        {
            *pixelPtr = color;
            (pixelPtr)++;
        }
    }
}

LRESULT CALLBACK windowProcessMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Switch on message typew
    switch (uMsg)
    {
        case WM_CREATE:
        {
            SetTimer(hwnd, MAIN_WINDOW_TIMER_ID, frameRate, NULL);
            return 0;
        }
        case WM_QUIT:
        case WM_DESTROY:
        {
            bQuit = true;
            break;
        }
        case WM_MOUSEMOVE:
        {
            mouseX = GET_X_LPARAM(lParam);
            mouseY = GET_Y_LPARAM(lParam);
        }
        case WM_TIMER:
        {
            RECT rect;
            GetClientRect(hwnd, &rect);
            bufferWidth = rect.right - rect.left;
            bufferHeight = rect.bottom - rect.top;

            int bufferSize = bufferWidth * bufferHeight * sizeof(unsigned int);

            if (bufferMemory)
            {
                VirtualFree(bufferMemory, 0, MEM_RELEASE);
            }
            bufferMemory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            bufferBmi.bmiHeader.biSize = sizeof(bufferBmi.bmiHeader);
            bufferBmi.bmiHeader.biWidth = bufferWidth;
            bufferBmi.bmiHeader.biHeight = bufferHeight;
            bufferBmi.bmiHeader.biPlanes = 1;
            bufferBmi.bmiHeader.biBitCount = 32;
            bufferBmi.bmiHeader.biCompression = BI_RGB;

            InvalidateRect(hwnd, NULL, FALSE);
            UpdateWindow(hwnd);
        }
        case WM_SIZE:
        {
            InvalidateRect(hwnd, NULL, FALSE);
            UpdateWindow(hwnd);
        } 
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Application::Application()
{

}

Application* Application::getAppInstance()
{
    if (instance == nullptr)
    {
        instance = new Application();
    }
    return instance;
}

void Application::init()
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = windowProcessMessage;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.hInstance     = m_hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.
    m_hwnd = CreateWindow(
        CLASS_NAME,                         // Window class
        L"MiniEngine",                      // Window text
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,   // Window style

        CW_USEDEFAULT,
        CW_USEDEFAULT,
        640,                                // Width
        480,                                // Height

        NULL,                               // Parent window    
        NULL,                               // Menu
        m_hInstance,                        // Instance handle
        NULL                                // Additional application data
    );
}

int Application::run()
{
    ShowWindow(m_hwnd, 1);

    HDC hdc = GetDC(m_hwnd);

    // Run the message loop.
    while (!bQuit)
    {
        static MSG message = { 0 };
        while (PeekMessage(&message, m_hwnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        fillColor(0x00796B);

        int staticPos = 100 + xOffset;
        if (staticPos > bufferWidth)
        {
            staticPos = staticPos % bufferWidth;
        }
        xOffset++;
        drawRect(staticPos, 50, 100, 100, 0xF57C00);
        drawRect(mouseX, bufferHeight - mouseY, 100, 100, 0xFDD835);

        StretchDIBits(
            hdc,
            0, 0,
            bufferWidth, bufferHeight,
            0, 0,
            bufferWidth, bufferHeight,
            bufferMemory,
            &bufferBmi,
            DIB_RGB_COLORS,
            SRCCOPY
        );
    };

    return 0;
}

Application *Application::instance = 0;