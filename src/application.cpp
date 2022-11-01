#include <chrono>
#include <thread>

#include "application.h"

MINI_USING_DIRECTIVE

#ifndef MAIN_WINDOW_TIMER_ID
#define MAIN_WINDOW_TIMER_ID 1001
#endif

// Global variables
static std::string MODEL_FILENAME = "../models/pumpkin.obj";

static bool     bIsRunning          = false;
static bool     bFlipFlop           = false;
static int      initWidth           = 512;     // Standard HD
static int      initHeight          = 512;

// Display options
static bool     bDrawFaces          = true;
static bool     bDrawEdges          = false;
static bool     bDrawVertices       = false;
static bool     bDisplayDebugText   = true;
static bool     bDisplayFps         = true;

// Keyboard input
static WORD     keyCode;
static WORD     keyFlags;

static bool     W_DOWN              = false;
static bool     A_DOWN              = false;
static bool     S_DOWN              = false;
static bool     D_DOWN              = false;
static bool     E_DOWN              = false;
static bool     Q_DOWN              = false;
static bool     SPACEBAR_DOWN       = false;
static float    MOUSE_WHEEL_DELTA   = 0.0;

static double   CAMERA_SPEED        = 0.00025;
static double   ROTATION            = 0.0;

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
            SetTimer(hwnd, MAIN_WINDOW_TIMER_ID, 1, NULL);
            ShowCursor(TRUE);
            return 0;
        }
        case WM_QUIT:
        case WM_DESTROY:
        {
            bIsRunning = true;
            break;
        }
        case WM_MOUSEWHEEL:
        {
            MOUSE_WHEEL_DELTA += GET_WHEEL_DELTA_WPARAM(wParam);
            break;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            keyCode  = LOWORD(wParam);
            keyFlags = HIWORD(lParam);

            switch (keyCode)
            {
                case '1': bDrawFaces = !bDrawFaces; break;
                case '2': bDrawEdges = !bDrawEdges; break;
                case '3': bDrawVertices = !bDrawVertices; break;
                case 'W': W_DOWN = false; break;
                case 'A': A_DOWN = false; break;
                case 'S': S_DOWN = false; break;
                case 'D': D_DOWN = false; break;
                case 'E': E_DOWN = false; break;
                case 'Q': Q_DOWN = false; break;
                case 'T': bDisplayDebugText = !bDisplayDebugText; break;
                case 'F': bDisplayFps = !bDisplayFps; break;
                case VK_ESCAPE: bIsRunning = false; break;
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
                case 'W': W_DOWN = true; break;
                case 'A': A_DOWN = true; break;
                case 'S': S_DOWN = true; break;
                case 'D': D_DOWN = true; break;
                case 'E': E_DOWN = true; break;
                case 'Q': Q_DOWN = true; break;
                case VK_ESCAPE: bIsRunning = false; break;
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

int deltaTime(int prev, int offset)
{
    return (std::clock() - prev) + offset;
}

void Application::displayPrintText()
{
    HDC hdc = GetDC(m_hwnd);
    auto text = PrintBuffer::getInstance()->getEntries();
    RECT rect;
    GetClientRect(m_hwnd, &rect);
    SetTextColor(hdc, (COLORREF) Color::white().hex());
    SetBkMode(hdc, TRANSPARENT);
    rect.left = 40;
    rect.top = 40;
    DrawText(hdc, text, -1, &rect, DT_NOCLIP);
}

void Application::displayFps()
{
    HDC hdc = GetDC(m_hwnd);
    double fps = 1000.0 / m_deltaTime;
    std::string fpsText = std::format("FPS: {}", std::floor(fps));
    std::wstring stemp = std::wstring(fpsText.begin(), fpsText.end());
    LPCWSTR text = stemp.c_str();
    RECT rect;
    GetClientRect(m_hwnd, &rect);
    SetTextColor(hdc, (COLORREF) Color::cyan().hex());
    SetBkMode(hdc, TRANSPARENT);
    rect.left = rect.right - 80;
    rect.top = 40;
    DrawText(hdc, text, -1, &rect, DT_NOCLIP);
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
    
    m_currentTime = getCurrentTime();

    // Load our mesh
    std::string filename = MODEL_FILENAME;
    Mesh mesh;
    MeshLoader::load(filename, mesh);

    // Move the camera to the default position
    m_buffer->camera()->move(Vector3(0, 0, -5));

    // Run the message loop.
    while (!bIsRunning)
    {
        PrintBuffer::clear();

        MOUSE_WHEEL_DELTA = 0.0; // Reset mouse delta

        double newTime = getCurrentTime();
        m_deltaTime = newTime - m_currentTime;
        m_currentTime = newTime;

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

        // Rotate the model
        ROTATION += CAMERA_SPEED * m_deltaTime;
        m_buffer->modelRotation = ROTATION;

        //// Move our camera
        double d = CAMERA_SPEED * m_deltaTime;
        Vector3 offset;
        Transform xform = m_buffer->camera()->getTransform();
        if (W_DOWN)
        {
            offset = Vector3(0, 0, d);
        }
        if (A_DOWN)
        {
            offset = Vector3(-d, 0, 0);
        }
        if (S_DOWN)
        {
            offset = Vector3(0, 0, -d);
        }
        if (D_DOWN)
        {
            offset = Vector3(d, 0, 0);
        }
        if (E_DOWN)
        {
            offset = Vector3(0, d, 0);
        }
        if (Q_DOWN)
        {
            offset = Vector3(0, -d, 0);
        }
        offset += xform.getTranslation();
        m_buffer->camera()->move(offset);

        if (MOUSE_WHEEL_DELTA != 0)
        {
            auto fov = m_buffer->camera()->getFieldOfView() - (MOUSE_WHEEL_DELTA / 240.0);
            m_buffer->camera()->setFieldOfView(fov);
        }

        // Bind vertex and index buffers to the Framebuffer
        m_buffer->bindTriangleBuffer(mesh.getTris());

        // Draw our scene geometry as triangles
        m_buffer->render();

        // Push our current RGB buffer to the display buffer
        m_buffer->allocateDisplayPtr();

        // Copy the memory buffer to the device context
        HDC hdc = GetDC(m_hwnd);
        StretchDIBits(
            hdc,
            0, 0,
            width, height,
            0, 0,
            width, height,
            m_buffer->getDisplayPtr(),
            m_buffer->getBitmapInfo(),
            DIB_RGB_COLORS,
            SRCCOPY
        );

        // Debug print to screen
        PrintBuffer::debugPrintToScreen("Vertex count: %i", mesh.getVertices().size());
        PrintBuffer::debugPrintToScreen("Triangle count: %i", mesh.getTris().size());

        if (bDisplayDebugText)
        {
            displayPrintText();
        }

        if (bDisplayFps)
        {
            displayFps();
        }

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