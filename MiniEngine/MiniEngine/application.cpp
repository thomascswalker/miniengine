#include "application.h"
#include <string>

#ifndef MAIN_WINDOW_TIMER_ID
    #define MAIN_WINDOW_TIMER_ID 1001
#endif

// Global variables
static bool     globalRunning       = false;
static UINT     globalFrameRate     = 1;        // 60 FPS
static int      initWidth           = 1280;     // Standard HD
static int      initHeight          = 720;

// Keyboard input
static WORD     keyCode;
static WORD     keyFlags;

// Temp globals
static int      playerX     = 0;
static int      playerY     = 0;
static int      forceUp     = 0;
static int      forceRight  = 0;

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
            SetTimer(hwnd, MAIN_WINDOW_TIMER_ID, globalFrameRate, NULL);
            return 0;
        }
        case WM_QUIT:
        case WM_DESTROY:
        {
            globalRunning = true;
            break;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            keyCode  = LOWORD(wParam);
            keyFlags = HIWORD(lParam);

            switch (keyCode)
            {
                case 'W': forceUp = 0; break;
                case 'A': forceRight = 0; break;
                case 'S': forceUp = 0; break;
                case 'D': forceRight = 0; break;
            }
            break;
        }
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            keyCode  = LOWORD(wParam);
            keyFlags = HIWORD(lParam);

            switch (keyCode)
            {
                case 'W': forceUp = -1; break;      // Invert
                case 'A': forceRight = -1; break;
                case 'S': forceUp = 1; break;       // Invert
                case 'D': forceRight = 1; break;
            }
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
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            app->setMousePos(x, y);
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

    // Run the message loop.
    while (!globalRunning)
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

        // Clear the framebuffer
        m_buffer->clear();

        // Render the UV gradient
        //m_buffer->renderGradient();

        // Paint a mouse cursor
        auto cursor = MColor(255, 255, 255);
        MCore::MRect cursorRect(m_mouseX - 10, m_mouseY - 10, 20, 20);
        m_buffer->fillRect(
            cursorRect.getMin().x(), // x0
            cursorRect.getMin().y(), // y0
            cursorRect.getMax().x(), // x1
            cursorRect.getMax().y(), // y1
            cursor                   // color
        );

        // Move our player
        playerX += forceRight;
        playerY += forceUp;
        playerX = MCore::clamp(&playerX, 0, width);
        playerY = MCore::clamp(&playerY, 0, height);

        // Paint our player
        auto player = MColor(50, 50, 235);
        m_buffer->fillRect(playerX, playerY, playerX + 50, playerY + 50, player);


        // Create a triangle
        Vector3 v1(0.0f, 0.5f, 0.0f);
        Vector3 v2(0.5f,  -0.5f, 0.0f);
        Vector3 v3(-0.5f, -0.5f, 0.0f);

        Vector2 v1_screen = m_buffer->worldToScreen(v1, Matrix4());
        Vector2 v2_screen = m_buffer->worldToScreen(v2, Matrix4());
        Vector2 v3_screen = m_buffer->worldToScreen(v3, Matrix4());

        auto triangle_color = MColor(0, 255, 50);
        m_buffer->fillRect(v1_screen.x(), v1_screen.y(),
                           v1_screen.x() + 50, v1_screen.y() + 50,
                           triangle_color);

        m_buffer->fillRect(v2_screen.x(), v2_screen.y(),
                           v2_screen.x() + 50, v2_screen.y() + 50,
                           triangle_color);

        m_buffer->fillRect(v3_screen.x(), v3_screen.y(),
                           v3_screen.x() + 50, v3_screen.y() + 50,
                           triangle_color);

        // Copy the memory buffer to the device context
        HDC hdc = GetDC(m_hwnd);
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
        ReleaseDC(m_hwnd, hdc);
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