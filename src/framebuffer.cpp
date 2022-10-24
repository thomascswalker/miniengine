#include "framebuffer.h"

MINI_USING_DIRECTIVE

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
    t.setTranslation(Vector3(0, 0, -5));
    m_camera.setTransform(t);
}

Framebuffer::~Framebuffer()
{
    clear();
}

void Framebuffer::allocate()
{
    // Clear the color buffer
    if (m_memoryBuffer)
    {
        VirtualFree(m_memoryBuffer, 0, MEM_RELEASE);
        m_memoryBuffer = nullptr;
    }

    // Calculate the new buffer size and allocate memory of that size
    int bufferSize = m_width * m_height * m_bytesPerPixel;

    // Allocate the memory buffer
    m_memoryBuffer = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    // Update the buffer BitmapInfo struct with the updated buffer size,
    // and width and height
    m_bufferBmi.bmiHeader.biSize = sizeof(m_bufferBmi.bmiHeader);
    m_bufferBmi.bmiHeader.biWidth = m_width;
    m_bufferBmi.bmiHeader.biHeight = -m_height; // When height is negative, it will invert the bitmap vertically.

    // Set the row length to the current window width * bytes per pixel (4)
    m_rowLength = m_width * m_bytesPerPixel;
}

void Framebuffer::clear()
{
    drawRect(0, 0,              // Origin
             m_width, m_height, // Width, height
             Color::black());   // Color
}

void Framebuffer::setSize(Size size)
{
    m_width = (int) size.width();
    m_height = (int) size.height();
    allocate();
}

void Framebuffer::setSize(int width, int height)
{
    m_width = width;
    m_height = height;
    allocate();
}

void Framebuffer::bindVertexBuffer(std::vector<Vertex> data)
{
    m_vertices = data;
}

void Framebuffer::bindIndexBuffer(std::vector<int> data)
{
    m_indices = data;
}

void Framebuffer::bindTriangleBuffer(std::vector<Triangle> data)
{
    m_triangles = data;
}

size_t Framebuffer::getNumVertices()
{
    return m_vertices.size();
}

size_t Framebuffer::getNumIndices()
{
    return m_indices.size();
}

size_t Framebuffer::getNumTriangles()
{
    return m_triangles.size();
}

bool Framebuffer::isPointInFrame(Vector2& p) const
{
    auto x = p.x();
    auto y = p.y();
    return (x > 0 && y > 0 && x < m_width && y < m_height);
}

bool Framebuffer::isRectInFrame(Rect& r) const
{
    return r.getMin().x() < m_width - 1.0   ||
           r.getMax().x() > 0.0             ||
           r.getMin().y() < m_height - 1.0  ||
           r.getMax().y() > 0.0;
};

void Framebuffer::setPixel(int x, int y, Color color, Buffer buffer)
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
            pixelPtr = (uint32*)m_memoryBuffer;
            break;
        }
    }

    Math::clamp(x, 0, m_width);
    Math::clamp(y, 0, m_height);
    uint32 offset = x + (y * m_width);
    pixelPtr += offset;
    *pixelPtr = color.hex();
}

void Framebuffer::setPixel(Vector2& v, Color color, Buffer buffer)
{
    int x = (int)v.x();
    int y = (int)v.y();
    setPixel(x, y, color, buffer);
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
            Vector2 point(x, y);
            setPixel(point, color);
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
                Vector2 point(x, y);
                setPixel(point, color);
            }
        }
    }
}

void Framebuffer::drawCircle(Vector2& v, int r, Color color)
{
    int x = (int)v.x();
    int y = (int)v.y();
    drawCircle(x, y, r, color);
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

    double step;

    double dx = maxX - minX;
    double dy = maxY - minY;

    auto adx = abs(dx);
    auto ady = abs(dy);
    step = (adx >= ady) ? adx : ady;

    dx = dx / step;
    dy = dy / step;

    double x = minX;
    double y = minY;
    int i = 1;

    while (i <= step)
    {
        Vector2 point(x, y);
        setPixel(point, color);

        x += dx;
        y += dy;
        i++;
    }
}

Vector3 Framebuffer::worldToScreen(Vector3& v)
{
    // Convert to normalized device coords
    Vector4 ndc = m_mvp * Vector4(v, 1.0);          // m_mvp is precalculated in Framebuffer::render()

    double x = ((ndc.x() + 1.0) * m_width) / 2.0;
    double y = ((ndc.y() + 1.0) * m_height) / 2.0;

    v.setX(x);
    v.setY(y);
    v.setZ(1.0 / ndc.z());                          // Invert Z

    return v;
}

// Returns the Z-depth of point P given a triangle (v1, v2, v3).
double Framebuffer::getDepth(Vector3& v1, Vector3& v2, Vector3& v3, Vector3& p)
{
    double area = Math::edge(v1, v2, v3);

    // Calculate depth
    double w1 = Math::edge(v2, v3, p);
    double w2 = Math::edge(v3, v1, p);
    double w3 = Math::edge(v1, v2, p);
    if (w1 < 0.0 && w2 < 0.0 && w3 < 0.0)
    {
        return DBL_MAX;
    }
    w1 /= area;
    w2 /= area;
    w3 /= area;

    return w1 * v1.z() + w2 * v2.z() + w3 * v3.z();
}

Rect Framebuffer::getBoundingBox(Vector3& v1, Vector3& v2, Vector3& v3)
{
    // Determine the screen bounding box
    std::vector<double> xList = { v1.x(), v2.x(), v3.x() };
    std::vector<double> yList = { v1.y(), v2.y(), v3.y() };

    auto minX = *std::min_element(std::begin(xList), std::end(xList));
    auto maxX = *std::max_element(std::begin(xList), std::end(xList));
    auto minY = *std::min_element(std::begin(yList), std::end(yList));
    auto maxY = *std::max_element(std::begin(yList), std::end(yList));

    Vector2 min(minX, minY);
    Vector2 max(maxX, maxY);

    return Rect(min, max);
}

// https://www.scratchapixel.com/code.php?id=26&origin=/lessons/3d-basic-rendering/rasterization-practical-implementation
void Framebuffer::drawTriangle(Vector3& v1, Vector3& v2, Vector3& v3)
{
    // Convert world-space to screenspace
    worldToScreen(v1);
    worldToScreen(v2);
    worldToScreen(v3);

    // Get the bounding box of the screen triangle
    Rect bounds = getBoundingBox(v1, v2, v3);

    // If the entire triangle is out of frame, skip
    if (!isRectInFrame(bounds))
    {
        return;
    }

    // Draw each pixel within the bounding box
    for (double x = bounds.getMin().x(); x < bounds.getMax().x(); x++)
    {
        for (double y = bounds.getMin().y(); y < bounds.getMax().y(); y++)
        {
            // Current pixel index
            int i = ((int) y * (int) m_width) + (int) x;

            // If the pixel is outside the frame entirely, we'll skip it
            Vector3 p(x + 0.5, y + 0.5, 0);
            if (!isPointInFrame(p))
            {
                continue;
            }

            // Get barycentric coordinates of triangle (uvw)
            Vector3 uvw = Triangle::getBarycentricCoords(v1, v2, v3, p);

            // If the total != 1.0, or all of the coord axes are less than 0,
            // we'll skip this (it's not in the triangle!)
            if (uvw.x() < 0 || uvw.y() < 0 || uvw.z() < 0)
            {
                continue;
            }

            // Calculate depth
            double z = getDepth(v1, v2, v3, p);

            // If the z-depth is greater (further back) than what's currently at this pixel, we'll
            // skip it.
            if (z > m_depthBuffer[i] || z < 0.0)
            {
                continue;
            }
            m_depthBuffer[i] = z; // Otherwise we'll set the current pixel Z to this depth

            // Calculate vertex colours
            double ar = 255, bg = 255, cb = 255;
            double ag = 0, ab = 0, br = 0, bb = 0, cr = 0, cg = 0;

            double r = uvw.x() * ar + uvw.y() * br + uvw.z() * cr;
            double g = uvw.x() * ag + uvw.y() * bg + uvw.z() * cg;
            double b = uvw.x() * ab + uvw.y() * bb + uvw.z() * cb;

            Color color = Color((int)r, (int)g, (int)b);
            setPixel(p, color, RGB);
        }   
    }
}

void Framebuffer::render()
{
    m_depthBuffer.clear();
    int depthBufferSize = (int)(m_width * m_height);
    for (int i = 0; i < depthBufferSize; i++)
    {
        m_depthBuffer.push_back(m_camera.getFarClip());
    }

    //Pre-compute the view/projection only once per frame, rather than for every vertex
    m_view = m_camera.getViewMatrix();                          //View matrix
    m_proj = m_camera.getProjectionMatrix(m_width, m_height);   // Projection matrix

    // Update MVP matrix
    Matrix4 model = makeRotationX(modelRotation) * makeRotationY(modelRotation);
    m_mvp = m_proj * m_view * model;

    for (auto t : m_triangles)
    {
        auto v1 = t.v1().getTranslation();
        auto v2 = t.v2().getTranslation();
        auto v3 = t.v3().getTranslation();

        drawTriangle(v1, v2, v3);
    }

    PrintBuffer::debugPrintToScreen("Pixel count: %i", m_depthBuffer.size());
}

bool operator == (const Framebuffer& f1, const Framebuffer& f2)
{
    return typeid(f1) == typeid(f2);
}

