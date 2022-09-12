#include "application.h"
#include <chrono>
#include <thread>

#ifndef MAIN_WINDOW_TIMER_ID
#define MAIN_WINDOW_TIMER_ID 1001
#endif

// Global variables
static bool     bIsRunning          = false;
static bool     bFlipFlop           = false;
static UINT     globalFrameRate     = 1;       // 60 FPS
static int      initWidth           = 720;     // Standard HD
static int      initHeight          = 720;
static int      tickRate            = 60;
static double   currentTime         = 0.0;

// Display options
static bool     bDrawFaces          = true;
static bool     bDrawEdges          = true;
static bool     bDrawVertices       = false;

// Keyboard input
static WORD     keyCode;
static WORD     keyFlags;

static bool     W_DOWN = false;
static bool     A_DOWN = false;
static bool     S_DOWN = false;
static bool     D_DOWN = false;
static bool     E_DOWN = false;
static bool     Q_DOWN = false;
static bool     SPACEBAR_DOWN = false;

static float ROTATION = 0.0f;

static double GEO_SIZE = 0.1;

// Create a pyramid
std::vector<Vertex> pVertices = {
    Vertex(0,  GEO_SIZE,  0),
    Vertex(-GEO_SIZE, -GEO_SIZE,  GEO_SIZE),
    Vertex(GEO_SIZE, -GEO_SIZE,  GEO_SIZE),
    Vertex(0,  GEO_SIZE,  0),
    Vertex(GEO_SIZE, -GEO_SIZE,  GEO_SIZE),
    Vertex(GEO_SIZE, -GEO_SIZE, -GEO_SIZE),
    Vertex(0,  GEO_SIZE,  0),
    Vertex(GEO_SIZE, -GEO_SIZE, -GEO_SIZE),
    Vertex(-GEO_SIZE, -GEO_SIZE, -GEO_SIZE),
    Vertex(0,  GEO_SIZE,  0),
    Vertex(-GEO_SIZE, -GEO_SIZE, -GEO_SIZE),
    Vertex(-GEO_SIZE, -GEO_SIZE,  GEO_SIZE)
};
std::vector<int> pIndices = {
    1, 0, 0, 1,
    0, 1, 0, 1,
    0, 0, 1, 1,
    1, 0, 0, 1,
    0, 0, 1, 1,
    0, 1, 0, 1,
    1, 0, 0, 1,
    0, 1, 0, 1,
    0, 0, 1, 1,
    1, 0, 0, 1,
    0, 0, 1, 1,
    0, 1, 0, 1

};

// Create a cube
std::vector<Vertex> vertices = {
    Vertex(-GEO_SIZE, -GEO_SIZE,  GEO_SIZE),   //0
    Vertex(GEO_SIZE, -GEO_SIZE,  GEO_SIZE),    //1
    Vertex(-GEO_SIZE,  GEO_SIZE,  GEO_SIZE),   //2
    Vertex(GEO_SIZE,  GEO_SIZE,  GEO_SIZE),    //3
    Vertex(-GEO_SIZE, -GEO_SIZE, -GEO_SIZE),   //4
    Vertex(GEO_SIZE, -GEO_SIZE, -GEO_SIZE),    //5
    Vertex(-GEO_SIZE,  GEO_SIZE, -GEO_SIZE),   //6
    Vertex(GEO_SIZE,  GEO_SIZE, -GEO_SIZE)     //7
};
std::vector<int> indices = {
    //Top
    2, 6, 7,
    2, 3, 7,

    //Bottom
    0, 4, 5,
    0, 1, 5,

    //Left
    0, 2, 6,
    0, 4, 6,

    //Right
    1, 3, 7,
    1, 5, 7,

    //Front
    0, 2, 3,
    0, 1, 3,

    //Back
    4, 6, 7,
    4, 5, 7
};

auto mesh = Mesh(vertices, indices);
//auto mesh = Mesh(pVertices, pIndices);

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
            ShowCursor(TRUE);
            return 0;
        }
        case WM_QUIT:
        case WM_DESTROY:
        {
            bIsRunning = true;
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
                //case VK_SPACE: SPACEBAR_DOWN = false; break;
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
                //case VK_SPACE: SPACEBAR_DOWN = true; break;
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

    currentTime = Core::getCurrentTime();

    auto m = Matrix4();
    auto camera = Camera();

    // Run the message loop.
    while (!bIsRunning)
    {
        double newTime = Core::getCurrentTime();
        double frameTime = newTime - currentTime;
        currentTime = newTime;

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

        //// Move our camera
        double d = 0.01f * frameTime;
        if (W_DOWN)
        {
            m_buffer->camera()->move(Vector3(0, 0, d));
        }
        if (A_DOWN)
        {
            m_buffer->camera()->move(Vector3(-d, 0, 0));
        }
        if (S_DOWN)
        {
            m_buffer->camera()->move(Vector3(0, 0, -d));
        }
        if (D_DOWN)
        {
            m_buffer->camera()->move(Vector3(d, 0, 0));
        }
        if (E_DOWN)
        {
            m_buffer->camera()->move(Vector3(0, d, 0));
        }
        if (Q_DOWN)
        {
            m_buffer->camera()->move(Vector3(0, -d, 0));
        }

        // Bind vertex and index buffers to the Framebuffer
        m_buffer->setVertexBufferData(mesh.getVertices());
        m_buffer->setIndexBufferData(mesh.getIndices());

        // Draw our scene geometry as triangles
        m_buffer->drawScene(bDrawFaces, bDrawEdges, bDrawVertices);

        // Draw a mouse cursor
        m_buffer->drawCircle(m_mouseX, m_mouseY, 6, Color::red());

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

        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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