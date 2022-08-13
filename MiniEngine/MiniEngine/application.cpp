#include "application.h"
#include <string>

#ifndef MAIN_WINDOW_TIMER_ID
    #define MAIN_WINDOW_TIMER_ID 1001
#endif

// Global variables
static bool bQuit      = false;
static float frameRate = 16.1667;   // 60 FPS
static int  initWidth  = 1280;      // Standard HD
static int  initHeight = 720;

LRESULT CALLBACK windowProcessMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto app = Application::getAppInstance();

    int width = LOWORD(lParam);
    int height = HIWORD(lParam);

    // Switch on message type
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
        case WM_SIZE:
        {
            app->setSize(width, height);
            InvalidateRect(hwnd, NULL, FALSE);
            UpdateWindow(hwnd);
            break;
        } 
        case WM_MOUSEMOVE:
        {
            POINT pt;
            pt.x = GET_X_LPARAM(lParam);

            int height = app->getFramebuffer()->getHeight();
            pt.y = height - GET_Y_LPARAM(lParam);

#if _DEBUG
            wchar_t buffer[256];
            wsprintfW(buffer, L"X: %d, Y: %d\n", pt.x, pt.y);
            OutputDebugString(buffer);
#endif

            app->setMousePos(pt.x, pt.y);
            break;
        }
        case WM_TIMER:
        {
            InvalidateRect(hwnd, NULL, FALSE);
            UpdateWindow(hwnd);

            break;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
        initWidth,                          // Width
        initHeight,                         // Height

        NULL,                               // Parent window    
        NULL,                               // Menu
        m_hInstance,                        // Instance handle
        NULL                                // Additional application data
    );

    // Initialize our framebuffer
    m_buffer = new Framebuffer(m_hwnd);
    m_buffer->setSize(initWidth, initHeight);
}

int Application::run()
{
    ShowWindow(m_hwnd, 1);

    HDC hdc = GetDC(m_hwnd);

    // Temporary variable for animating our rectangle
    int xOffset = 0;

    // Run the message loop.
    while (!bQuit)
    {
        RECT clientRect;
        GetClientRect(m_hwnd, &clientRect);

        static MSG message = { 0 };
        while (PeekMessage(&message, m_hwnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        int width = m_buffer->getWidth();
        int height = m_buffer->getHeight();

        // Paint background
        auto background  = MColor(0x00796B);
        m_buffer->fillRect(0, 0, width, height, background);

        // Paint a rectangle
        auto box = MColor(255, 200, 50);
        m_buffer->fillRect(0, 0, 250, 250, box);

        // Paint a mouse cursor
        auto cursor = MColor(255, 0, 0);
        m_buffer->fillRect(
            m_mouseX - 10, // x0
            m_mouseY - 10, // y0
            m_mouseX + 10, // x1
            m_mouseY + 10, // y1
            cursor         // color
        );

        // Copy the memory buffer to the device context
        StretchDIBits(
            hdc,
            0, 0,
            width, height,
            0, 0,
            width, height,
            m_buffer->getMemoryPtr(),
            m_buffer->getBitmapInfo(),
            DIB_RGB_COLORS,
            SRCCOPY
        );
    };

    return 0;
}

void Application::setSize(int width, int height)
{
    if (m_buffer == nullptr)
    {
        m_width = width;
        m_height = height;
    }
    else
    {
        m_width = width;
        m_height = height;
        m_buffer->setSize(width, height);
    }
}

void Application::setMousePos(int x, int y)
{
    m_mouseX = x;
    m_mouseY = y;
}

Application *Application::instance = 0;