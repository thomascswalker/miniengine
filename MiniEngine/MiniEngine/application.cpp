#include "application.h"

#ifndef MAIN_WINDOW_TIMER_ID
#define MAIN_WINDOW_TIMER_ID 1001
#endif

#ifndef DRAW_FACES
#define DRAW_FACES true
#endif

#ifndef DRAW_WIREFRAME
#define DRAW_WIREFRAME true
#endif

#ifndef DRAW_VERTICES
#define DRAW_VERTICES true
#endif

// Global variables
static bool     globalRunning       = false;
static UINT     globalFrameRate     = 1;       // 60 FPS
static int      initWidth           = 300;     // Standard HD
static int      initHeight          = 300;
static int      tickRate            = 60;
static double   currentTime         = 0.0;
static bool     bFlipFlop = false;

// Display options
static bool     bDrawFaces          = true;
static bool     bDrawEdges          = true;
static bool     bDrawVertices       = false;

// Keyboard input
static WORD     keyCode;
static WORD     keyFlags;

// Temp globals
static int      playerX     = 0;
static int      playerY     = 0;
static int      forceUp     = 0;
static int      forceRight  = 0;

static float ROTATION = 0.0f;

// Create a triangle
std::vector<Vertex> vertices = {
    Vertex(-0.5, -0.5,  0.5), //0
    Vertex(0.5, -0.5,  0.5), //1
    Vertex(-0.5,  0.5,  0.5), //2
    Vertex(0.5,  0.5,  0.5), //3
    Vertex(-0.5, -0.5, -0.5), //4
    Vertex(0.5, -0.5, -0.5), //5
    Vertex(-0.5,  0.5, -0.5), //6
    Vertex(0.5,  0.5, -0.5)  //7
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
                case '1': bDrawFaces = !bDrawFaces; break;
                case '2': bDrawEdges = !bDrawEdges; break;
                case '3': bDrawVertices = !bDrawVertices; break;
                //case 'W': forceUp = 0; break;
                //case 'A': forceRight = 0; break;
                //case 'S': forceUp = 0; break;
                //case 'D': forceRight = 0; break;
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

    auto m = Matrices::Matrix4();

    // Run the message loop.
    while (!globalRunning)
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

        // Rotate our mesh
        m.rotateZ(ROTATION);
        ROTATION += 0.001f * frameTime;
        
        // Bind vertex and index buffers to the Framebuffer
        m_buffer->setVertexBufferData(mesh.getVertices(Coordinates::World));
        m_buffer->setIndexBufferData(mesh.getIndices());

        // Draw our scene geometry as triangles
        m_buffer->drawScene(m, bDrawFaces, bDrawEdges, bDrawVertices);

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