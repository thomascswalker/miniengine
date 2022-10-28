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
    m_camera = new Camera();
    auto t = m_camera->getTransform();
    t.setTranslation(Vector3(0, 0, -5));
    m_camera->setTransform(t);
}

Framebuffer::~Framebuffer()
{
    clear();
}

void Framebuffer::allocate()
{
    // Clear the color buffer
    if (m_colorBuffer)
    {
        VirtualFree(m_colorBuffer, 0, MEM_RELEASE);
        m_colorBuffer = nullptr;
    }

    // Calculate the new buffer size and allocate memory of that size
    int bufferSize = m_width * m_height;

    // Allocate the memory buffer
    m_colorBuffer = VirtualAlloc(0, bufferSize * sizeof(int), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    // Update the buffer BitmapInfo struct with the updated buffer size,
    // and width and height
    m_bufferBmi.bmiHeader.biSize = sizeof(m_bufferBmi.bmiHeader);
    m_bufferBmi.bmiHeader.biWidth = m_width;
    m_bufferBmi.bmiHeader.biHeight = -m_height; // When height is negative, it will invert the bitmap vertically.

    // Set the row length to the current window width * bytes per pixel (4)
    m_rowLength = m_width * m_bytesPerPixel;

    // Clear the color buffer
    if (m_zBuffer)
    {
        VirtualFree(m_zBuffer, 0, MEM_RELEASE);
        m_zBuffer = nullptr;
    }

    // Allocate the Z-buffer
    m_zBuffer = VirtualAlloc(0, bufferSize * sizeof(double), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

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

template <typename T>
void Framebuffer::setPixel(int x, int y, T value, Buffer buffer)
{
    if (x < 0 || y < 0 || x > m_width || y > m_height)
    {
        return;
    }
    T* pixelPtr = 0;

    switch (buffer)
    {
        default:
        case RGB:
        {
            pixelPtr = (T*) m_colorBuffer;
            break;
        }
        case DEPTH:
        {
            pixelPtr = (T*) m_zBuffer;
            break;
        }
    }

    Math::clamp(x, 0, m_width);
    Math::clamp(y, 0, m_height);
    T offset = x + (y * m_width);
    pixelPtr += offset;
    *pixelPtr = value;
}

template <typename T>
T* Framebuffer::getPixel(int x, int y, Buffer buffer)
{
    switch (buffer)
    {
    default:
    case RGB:
    {
        return (T*) m_colorBuffer;
    }
    case DEPTH:
    {
        return (T*) m_zBuffer;
    }
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
            Vector2 point(x, y);
            setPixel(x, y, color.hex(), RGB);
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
                setPixel(x, y, color.hex(), RGB);
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
        setPixel(x, y, color.hex(), RGB);

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

    double minX = *std::min_element(std::begin(xList), std::end(xList));
    double maxX = *std::max_element(std::begin(xList), std::end(xList));
    double minY = *std::min_element(std::begin(yList), std::end(yList));
    double maxY = *std::max_element(std::begin(yList), std::end(yList));

    Vector2 min(minX, minY);
    Vector2 max(maxX, maxY);

    return Rect(min, max);
}

// https://www.scratchapixel.com/code.php?id=26&origin=/lessons/3d-basic-rendering/rasterization-practical-implementation
void Framebuffer::drawTriangle(Vector3& v1, Vector3& v2, Vector3& v3)
{
    auto wv1 = m_model * v1;
    auto wv2 = m_model * v2;
    auto wv3 = m_model * v3;
    Vector3 normal = Triangle::getNormal(wv1, wv2, wv3);
    normal.normalize();

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
            // skip it. Also skip if we're outside of the near/far clip.
            double currentZ = *getPixel<double>(x, y, RGB);
            if (z > currentZ || z < m_camera->getNearClip() || z > m_camera->getFarClip())
            {
                continue;
            }
            setPixel(x, y, z, DEPTH); // Otherwise we'll set the current pixel Z to this depth
            //
            //// Calculate normal
            //Vector3 viewDirection = -m_camera->getForward();
            //viewDirection.normalize();

            //double facingRatio = Math::dot(normal, viewDirection);
            //double factor = GAMMA_CORRECT(facingRatio * 255.0);
            //double finalColor = Math::clamp(factor, 0.0, 255.0);

            //// Set final color in RGB buffer
            //Color color = Color((int) finalColor, 100, 100.0);
            //setPixel<uint32>(x, y, color.hex(), RGB);
        }   
    }
}

void Framebuffer::render()
{
    allocate();


    //Pre-compute the view/projection only once per frame, rather than for every vertex
    m_view = m_camera->getViewMatrix();                          //View matrix
    m_proj = m_camera->getProjectionMatrix(m_width, m_height);   // Projection matrix

    // Update MVP matrix
    m_model = makeRotationY(modelRotation);
    m_mvp = m_proj * m_view * m_model;

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

