#include <chrono>
#include <thread>

#include "application.h"

#ifndef MAIN_WINDOW_TIMER_ID
#define MAIN_WINDOW_TIMER_ID 1001
#endif

#define MIN_ZOOM_DISTANCE 2.0

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

// Global variables
static bool     bIsRunning          = false;
static bool     bFlipFlop           = false;

// Display options
static bool     bDrawFaces          = true;
static bool     bDrawEdges          = false;
static bool     bDrawVertices       = false;
static bool     bDisplayDebugText   = true;
static bool     bDisplayFps         = true;

static bool     MOUSE_DOWN          = false;
static bool     W_DOWN              = false;
static bool     A_DOWN              = false;
static bool     S_DOWN              = false;
static bool     D_DOWN              = false;
static bool     E_DOWN              = false;
static bool     Q_DOWN              = false;
static bool     O_DOWN              = false;
static bool     SPACEBAR_DOWN       = false;
static float    MOUSE_WHEEL_DELTA   = 0.0;

static double   CAMERA_SPEED        = 0.0025;

static double   ROTATION            = 0.0;
static double   ROTATION_SPEED      = 0.25;

Rotation MODEL_ROTATION = Rotation();

LRESULT CALLBACK windowProcessMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto app = Application::getAppInstance();

    WORD keyCode;
    WORD keyFlags;

    // Switch on message type
    switch (uMsg)
    {
        // Window
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

        // Mouse
        case WM_MOUSEMOVE:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            app->setMousePos(x, y);
            break;
        }
        case WM_LBUTTONDOWN:
        {
            MOUSE_DOWN = true;
            Point mousePos = app->getMousePos();
            app->setMouseClickPos(mousePos.x, mousePos.y);
            break;
        }
        case WM_LBUTTONUP:
        {
            MOUSE_DOWN = false;
            app->setMouseClickPos(-1, -1);
            break;
        }
        case WM_MOUSEWHEEL:
        {
            MOUSE_WHEEL_DELTA += GET_WHEEL_DELTA_WPARAM(wParam);
            break;
        }

        // Keyboard
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
                case 'O': O_DOWN = false; break;
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
                case 'O': O_DOWN = true; break;
                case VK_ESCAPE: bIsRunning = false; break;
            }
            break;
        }

        // Resize
        case WM_SIZE:
        {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);

            app->setSize(width, height);
            InvalidateRect(hwnd, NULL, FALSE);
            UpdateWindow(hwnd);
            
            auto cursor = LoadCursor(NULL, IDC_SIZENESW);
            SetCursor(cursor);

            break;
        } 

        // Timer called every ms to update
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
    SetTextColor(hdc, (COLORREF) Color::yellow().hex());
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
        DEFAULT_WINDOW_WIDTH,               // Width
        DEFAULT_WINDOW_HEIGHT,              // Height

        NULL,                               // Parent window    
        NULL,                               // Menu
        m_hInstance,                        // Instance handle
        NULL                                // Additional application data
    );

    // Initialize our framebuffer
    m_buffer = new Framebuffer(m_hwnd);
    m_buffer->setSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
}

int Application::run()
{
    ShowWindow(m_hwnd, 1);
    
    m_currentTime = getCurrentTime();

    // Move the camera to the default position
    m_buffer->getCamera()->move(Vector3(0, 0, 5.0));

    // Run the message loop.
    while (!bIsRunning)
    {
        // Clear the screen
        InvalidateRect(m_hwnd, NULL, TRUE);

        PrintBuffer::clear();

        // Print instructions
        PrintBuffer::debugPrintToScreen("O: Load a .obj file");
        PrintBuffer::debugPrintToScreen("T: Toggle text display");
        PrintBuffer::debugPrintToScreen("Left click + move: Orbit around");
        PrintBuffer::debugPrintToScreen("Middle mouse scroll: Zoom in/out\n");

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

        // Load our mesh
        if (O_DOWN)
        {
            if (!loadModel())
            {
                continue;
            }
            if (!loadShader())
            {
                continue;
            }
            O_DOWN = false;
        }

        // Arcball rotation
        if (MOUSE_DOWN)
        {
            onMouseDown();
        }
        else
        {
            auto cursor = LoadCursor(NULL, IDC_ARROW);
            SetCursor(cursor);
        }

        // Mouse scroll
        if (MOUSE_WHEEL_DELTA != 0)
        {
            onMouseScroll();
        }

        // Bind vertex and index buffers to the Framebuffer
        auto tris = m_staticMesh->getMesh()->getTris();
        m_buffer->bindTriangleBuffer(tris);

        // Draw our scene geometry as triangles
        m_buffer->render();

        // Push our current RGB buffer to the display buffer
        m_buffer->allocateDisplayPtr();

        // Copy the memory buffer to the device context
        HDC hdc = GetDC(m_hwnd);
        HDC src = CreateCompatibleDC(hdc);
        SelectObject(src, m_buffer->getBitmap());
        BitBlt(
            hdc,                                         // Target device context
            0, 0,                                        // Target start coords
            m_buffer->getWidth(), m_buffer->getHeight(), // Target size
            src,                                         // Source device context
            0, 0,                                        // Source start coords
            SRCCOPY                                      // Method
        );

        // Debug print to screen
        if (bDisplayDebugText)
        {
            displayPrintText();
        }

        if (bDisplayFps)
        {
            displayFps();
        }

        ReleaseDC(m_hwnd, hdc);

        setMouseLastPos(m_mousePos.x, m_mousePos.y);
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

bool Application::loadModel()
{
    std::string filename;
    if (!getOpenFilename(FILE_FILTER_OBJ, filename))
    {
        return false;
    }

    Mesh* mesh = loadMeshFile(filename);
    m_staticMesh->setMesh(mesh);

    return true;
}

bool Application::loadShader()
{
    //std::string filename;
    //if (!getOpenFilename(FILE_FILTER_SHADER, filename))
    //{
    //    return false;
    //}

    //StandardShader* shader = loadShaderFile(filename);
    //m_buffer->setPixelShader(shader);

    return true;
}

void Application::onMouseDown()
{
    Vector3 position = m_buffer->getCamera()->getTranslation();
    Vector3 target = m_buffer->getCamera()->getTarget();

    // Get original orientation
    Vector3 orientation = position - target;
    orientation.normalize();

    // Get distance from position to target
    double length = distance(position, target);

    // Delta
    int dx = m_mousePos.x - m_mouseLastPos.x;
    int dy = m_mousePos.y - m_mouseLastPos.y;

    if (dx != 0.0 || dy != 0.0)
    {
        auto cursor = LoadCursor(NULL, IDC_HAND);
        SetCursor(cursor);

        // Scale delta x/y
        double scaleX = (double) abs(dx) / (double) m_buffer->getWidth();
        double scaleY = (double) abs(dy) / (double) m_buffer->getHeight();

        // Angle (in radians)
        double xRot = (double) dx * scaleX * ROTATION_SPEED;
        double yRot = (double) dy * scaleY * ROTATION_SPEED;

        Matrix4 rx = makeRotationX(yRot);
        Matrix4 ry = makeRotationY(-xRot);
        Matrix4 rz = makeRotationZ(0.0);

        Vector3 d = position - target;
        d.normalize();
        Vector3 t = rx * ry * d * length;

        m_buffer->getCamera()->setTranslation(t);
    }
}

void Application::onMouseMove()
{

}

void Application::onMouseScroll()
{
    double delta = MOUSE_WHEEL_DELTA / 240.0;
    Vector3 cameraTarget = m_buffer->getCamera()->getTarget();
    Vector3 cameraPosition = m_buffer->getCamera()->getTranslation();

    // Get camera orientation (normal)
    Vector3 normal = cameraTarget - cameraPosition;
    normal.normalize();
    
    // Calculate new camera position given the current camera position and adding
    // the normalized camera orientation and delta multiplier (how much we're scrolling).
    Vector3 newPosition = cameraPosition + (normal * delta);
    double dist = distance(newPosition, cameraTarget);

    // Handle minimum zoom so we can't go past the origin
    if (dist < MIN_ZOOM_DISTANCE)
    {
        return;
    }

    // Set the new position of the camera
    m_buffer->getCamera()->setTranslation(newPosition);
}


Application *Application::instance = 0;

MINI_NAMESPACE_CLOSE