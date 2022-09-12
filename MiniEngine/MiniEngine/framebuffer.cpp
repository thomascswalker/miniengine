#include "framebuffer.h"

using namespace Core;

Framebuffer::Framebuffer(HWND hwnd)
    : m_hwnd(hwnd)
{
    // Set BitmapInfo struct default values
    m_bufferBmi.bmiHeader.biPlanes = 1;
    m_bufferBmi.bmiHeader.biBitCount = 32;
    m_bufferBmi.bmiHeader.biCompression = BI_RGB;

    // Do an initial allocation of the framebuffer
    allocate();

    // Create a new default camera
    m_camera = Camera();
    auto t = m_camera.getTransform();
    t.setTranslation(Vector3(0, 5, -20));
    m_camera.setTransform(t);
}

Framebuffer::~Framebuffer()
{
    
}

void Framebuffer::allocate()
{
    // Clear the memory buffer
    if (m_colorBuffer)
    {
        VirtualFree(m_colorBuffer, 0, MEM_RELEASE);
        m_colorBuffer = nullptr;
    }

    // Calculate the new buffer size and allocate memory of that size
    int bufferSize = m_width * m_height * m_bytesPerPixel;

    // Allocate the memory buffer
    m_colorBuffer = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    // Update the buffer BitmapInfo struct with the updated buffer size,
    // and width and height
    m_bufferBmi.bmiHeader.biSize = sizeof(m_bufferBmi.bmiHeader);
    m_bufferBmi.bmiHeader.biWidth = m_width;
    m_bufferBmi.bmiHeader.biHeight = -m_height; // When height is negative, it will invert the bitmap vertically.

    // Set the row length to the current window width * bytes per pixel (4)
    m_rowLength = m_width * m_bytesPerPixel;
}

void Framebuffer::setSize(Core::Size size)
{
    m_width = size.width();
    m_height = size.height();
    allocate();
}

void Framebuffer::setSize(int width, int height)
{
    m_width = width;
    m_height = height;
    allocate();
}

void Framebuffer::setVertexBufferData(std::vector<Vertex> data)
{
    m_vertices = data;
}

void Framebuffer::setIndexBufferData(std::vector<int> data)
{
    m_indices = data;
}

int Framebuffer::getNumVertices()
{
    return m_numVertices;
}

int Framebuffer::getNumIndices()
{
    return m_numIndices;
}

/*
https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/building-basic-perspective-projection-matrix
*/
Vector2 Framebuffer::vertexToScreen(Vertex vertex)
{
    // Projection matrix
    Matrix4 proj  = m_camera.getProjectionMatrix(m_width, m_height);;
    
    // View matrix
    auto cameraXForm = m_camera.getTransform();
    auto up = Vector3(0, 1, 0);
    auto at = Vector3(0, 0, 0);

    Matrix4 view = lookAt(cameraXForm.getTranslation(), at, up);

    // Vertex position
    Vector4 pos = Vector4(vertex.getTranslation(), 1.0);

    // MVP
    Vector4 vtx = proj * view * pos;
    vtx.setW(1);

    // Get X screenspace coordinate
    int w1 = m_width - 1;
    int w2 = (vtx.x() + 1) * 0.5 * m_width;
    int x = w1 < w2 ? w1 : w2;
    x = abs(x);

    // Get Y screenspace coordinate
    int h1 = m_height - 1;
    int h2 = 1 - ((vtx.y() + 1) * 0.5) * m_height;
    int y = h1 < h2 ? h1 : h2;
    y = abs(y);

    // Clamp screenspace coordinates
    //x = Math::clamp(x, 0, m_width);
    //y = Math::clamp(y, 0, m_height);

    return Vector2(x, y);
}

bool Framebuffer::isPointInFrame(Vector2& p) const
{
    int x = p.x();
    int y = p.y();
    return (x > 0 && y > 0 && x < m_width && y < m_height);
}

void Framebuffer::clear()
{
    drawRect(0, 0,              // Origin
             m_width, m_height, // Width, height
             Color::black());   // Color
}

void Framebuffer::setPixel(int x, int y, Color color, Buffer buffer = Buffer::RGB)
{
    if (x < 0 || y < 0 || x > m_width || y > m_height)
    {
        return;
    }
    uint32* pixelPtr = 0;

    switch (buffer)
    {
        default:
        case RGB:
        {
            pixelPtr = (uint32*)m_colorBuffer;
            break;
        }
        case DEPTH:
        {
            pixelPtr = (uint32*)m_depthBuffer;
            break;
        }
    }

    Math::clamp(x, 0, m_width);
    Math::clamp(y, 0, m_height);
    uint32 offset = x + (y * m_width);
    pixelPtr += offset;
    *pixelPtr = color.hex();
}

void Framebuffer::drawGradient()
{
    // The start position of the current row
    uint8 *rowPtr = (uint8*)m_colorBuffer;

    for (int y = 0; y < m_height; y++)
    {
        // Initial pointer position in our memory buffer
        uint32 *pixel = (uint32*)rowPtr;
        for (int x = 0; x < m_width; x++)
        {
            uint8 red = x;      // Red channel gradient getting brighter left -> right
            uint8 green = y;    // Green channel gradient getting brighter top -> down
            uint8 blue = 0;     // No blue
            auto color = Color(red, green, blue);

            // Set the color at this position in memory
            *pixel++ = color.hex();
        }

        rowPtr += m_rowLength;
    }
}

void Framebuffer::drawRect(int x0, int y0, int x1, int y1, Color color)
{
    x0 = Math::clamp(x0, 0, m_width);
    x1 = Math::clamp(x1, 0, m_width);
    
    y0 = Math::clamp(y0, 0, m_height);
    y1 = Math::clamp(y1, 0, m_height);

    // For each pixel...
    for (int y = y0; y < y1; y++)
    {
        for (int x = x0; x < x1; x++)
        {
            // Set the color at this position in memory
            setPixel(x, y, color);
        }
    }
}

void Framebuffer::drawCircle(int cx, int cy, int r, Color color)
{
    // Top left
    int x0 = cx - r;
    int y0 = cy - r;

    // Bottom right
    int x1 = cx + r;
    int y1 = cy + r;

    // Clamp based on buffer width/height
    x0 = Math::clamp(x0, 0, m_width);
    y0 = Math::clamp(y0, 0, m_height);

    x1 = Math::clamp(x1, 0, m_width);
    y1 = Math::clamp(y1, 0, m_height);

    auto rsqr = pow(r, 2);

    for (int y = y0; y < y1; y++)
    {
        for (int x = x0; x < x1; x++)
        {
            auto dx = x - cx;
            auto dy = y - cy;
            if (pow(dx, 2) + pow(dy, 2) <= rsqr)
            {
                setPixel(x, y, color);
            }
        }
    }
}

void Framebuffer::drawTri(Vector2& v1, Vector2& v2, Vector2& v3, Color color)
{
    // Determine the min/max threshold for drawing
    std::vector<double> xList = {v1.x(), v2.x(), v3.x()};
    std::vector<double> yList = {v1.y(), v2.y(), v3.y()};

    auto minX = *std::min_element(std::begin(xList), std::end(xList));
    auto maxX = *std::max_element(std::begin(xList), std::end(xList));
    auto minY = *std::min_element(std::begin(yList), std::end(yList));
    auto maxY = *std::max_element(std::begin(yList), std::end(yList));

    for (int y = minY; y < maxY; y++)                       // Bottom to top
    {
        for (int x = minX; x < maxX; x++)                   // Left to right
        {
            Vector2 point(x,y);                             // Point at current x, y
            if (!isPointInFrame(point))
            {
                continue;
            }
            if (Math::isPointInTriangle(point, v1, v2, v3)) // Is this point in our triangle?
            {
                setPixel(x, y, color);                      // If it is, colour here
            }
        }
    }
}

/*
https://en.wikipedia.org/wiki/Line_drawing_algorithm
*/
void Framebuffer::drawLine(Vector2& v1, Vector2& v2, Color color)
{
    std::vector<double> xList = { v1.x(), v2.x() };
    std::vector<double> yList = { v1.y(), v2.y() };

    auto minX = *std::min_element(std::begin(xList), std::end(xList));
    auto maxX = *std::max_element(std::begin(xList), std::end(xList));
    auto minY = *std::min_element(std::begin(yList), std::end(yList));
    auto maxY = *std::max_element(std::begin(yList), std::end(yList));

    float step;

    float dx = maxX - minX;
    float dy = maxY - minY;

    auto adx = abs(dx);
    auto ady = abs(dy);
    step = (adx >= ady) ? adx : ady;

    dx = dx / step;
    dy = dy / step;

    float x = minX;
    float y = minY;
    int i = 1;

    while (i <= step)
    {
        setPixel(x, y, color);

        x += dx;
        y += dy;
        i++;
    }
}

void Framebuffer::drawScene(bool bDrawFaces, bool bDrawEdges, bool bDrawVertices)
{

    //Matrix4 cameraMatrix = m_camera.getTransform().getMatrix();
    //Core::print("------\nCamera Matrix:\n%s\r", cameraMatrix.toString().c_str());

    for (int i = 0; i < m_indices.size(); i++)
    {
        // Get vertex indices
        int idx1 = m_indices[i]; i++;
        int idx2 = m_indices[i]; i++;
        int idx3 = m_indices[i];

        // Get vertexes from indices
        Vertex vtx1 = m_vertices[idx1];
        Vertex vtx2 = m_vertices[idx2];
        Vertex vtx3 = m_vertices[idx3];
        
        // Convert world pos to screen pos for each vertex
        Vector2 vtx1s = vertexToScreen(vtx1);
        Vector2 vtx2s = vertexToScreen(vtx2);
        Vector2 vtx3s = vertexToScreen(vtx3);

        //if (!isPointInFrame(vtx1s) || !isPointInFrame(vtx2s) || !isPointInFrame(vtx2s))
        //{
        //    continue;
        //}

        // Draw each face
        if (bDrawFaces)
        {
            drawTri(vtx1s, vtx2s, vtx3s, Color::white());
        }

        // Draw each edge
        if (bDrawEdges)
        {
            drawLine(vtx1s, vtx2s, Color::blue());
            drawLine(vtx1s, vtx3s, Color::blue());
            drawLine(vtx2s, vtx3s, Color::blue());
        }

        // Draw each vertex
        if (bDrawVertices)
        {
            drawCircle(vtx1s.x(), vtx1s.y(), 3, Color::orange());
            drawCircle(vtx2s.x(), vtx1s.y(), 3, Color::orange());
            drawCircle(vtx3s.x(), vtx1s.y(), 3, Color::orange());
        }
    }
}

bool operator == (const Framebuffer& f1, const Framebuffer& f2)
{
    return typeid(f1) == typeid(f2);
}

