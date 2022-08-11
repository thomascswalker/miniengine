#include "application.h"
#include <string>

#ifndef MAIN_WINDOW_TIMER_ID
    #define MAIN_WINDOW_TIMER_ID 1001
#endif

// Global variables
static bool bQuit      = false;
static int  initWidth  = 1280;
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
            SetTimer(hwnd, MAIN_WINDOW_TIMER_ID, 16.667, NULL);
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
            //mouseX = GET_X_LPARAM(lParam);
            //mouseY = GET_Y_LPARAM(lParam);
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
        static MSG message = { 0 };
        while (PeekMessage(&message, m_hwnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        // Paint a pink background
        auto pink  = MColor(0xFF796B);
        m_buffer->fillRect(0, 0, m_buffer->getWidth(), m_buffer->getHeight(), pink);

        // Paint a turquoise rectangle which we'll offset every iteration
        int staticX = 100 + xOffset;
        if (staticX > m_buffer->getWidth())
        {
            staticX = staticX % m_buffer->getWidth();
        }
        xOffset++;

        auto turqouise = MColor(0x00796B);
        m_buffer->fillRect(staticX, 150, staticX + 350, 600, turqouise);

        // Copy the memory buffer to the device context
        StretchDIBits(
            hdc,
            0, 0,
            m_buffer->getWidth(), m_buffer->getHeight(),
            0, 0,
            m_buffer->getWidth(), m_buffer->getHeight(),
            m_buffer->getMemoryBuffer(),
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

Application *Application::instance = 0;