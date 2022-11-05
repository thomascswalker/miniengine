#include "framebuffer.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

Framebuffer::Framebuffer(HWND hwnd)
    : m_hwnd(hwnd)
{
    // Set BitmapInfo struct default values
    m_bufferBmi.bmiHeader.biPlanes = 1;
    m_bufferBmi.bmiHeader.biBitCount = 32;
    m_bufferBmi.bmiHeader.biCompression = BI_RGB;

    for (const char* name : CHANNELS)
    {
        m_channels[name] = new Channel(name, m_width, m_height);
    }

    // Create a new default camera
    m_camera = Camera();
}

Framebuffer::~Framebuffer()
{
    //clear()/*;*/
}

void Framebuffer::setSize(Size size)
{
    setSize(size.width(), size.height());
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
    // Calculate area of this triangle
    double a = area(v1, v2, v3);

    // Calculate depth
    double w1 = area(v2, v3, p);
    double w2 = area(v3, v1, p);
    double w3 = area(v1, v2, p);
    if (w1 < 0.0 && w2 < 0.0 && w3 < 0.0)
    {
        return DBL_MAX;
    }
    w1 /= a;
    w2 /= a;
    w3 /= a;

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
    // Calculate normal
    Vector3 wv1 = m_model * v1;
    Vector3 wv2 = m_model * v2;
    Vector3 wv3 = m_model * v3;
    Vector3 normal = Triangle::getNormal(wv1, wv2, wv3);
    normal.normalize();

    // Calculate view direction
    Vector3 forward = -m_camera.getForward();
    forward.normalize();

    Vector3 up = Vector3::up();
    up.normalize();

    Vector3 right = -Vector3::right();
    right.normalize();

    // Calculate facing ratio
    double facingRatio = dot(normal, forward);
    double upRatio = dot(normal, up);
    double rightRatio = dot(normal, right);

    // Normalize facing ratio from -1 => 1 to 0 => 1
    facingRatio = normalizeNew(&facingRatio, -1.0, 1.0, 0.0, 1.0);
    upRatio = normalizeNew(&upRatio, -1.0, 1.0, 0.0, 1.0);
    rightRatio = normalizeNew(&rightRatio, -1.0, 1.0, 0.0, 1.0);

    Vector3 cameraNormal(rightRatio, upRatio, facingRatio);

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

    Channel* rChannel = m_channels[CHANNEL_R];
    Channel* gChannel = m_channels[CHANNEL_G];
    Channel* bChannel = m_channels[CHANNEL_B];
    Channel* zChannel = m_channels[CHANNEL_Z];

    // Draw each pixel within the bounding box
    for (double y = bounds.getMin().y(); y < bounds.getMax().y(); y++)
    {
        for (double x = bounds.getMin().x(); x < bounds.getMax().x(); x++)
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
            double currentZ = m_channels[CHANNEL_Z]->getPixel(x, y);
            if (z > currentZ || z < m_camera.getNearClip() || z > m_camera.getFarClip())
            {
                continue;
            }
            zChannel->setPixel(x, y, z); // Otherwise we'll set the current pixel Z to this depth
            
            // Store normals in channel
            m_channels[CHANNEL_NORMAL_R]->setPixel(x, y, cameraNormal.x());
            m_channels[CHANNEL_NORMAL_G]->setPixel(x, y, cameraNormal.y());
            m_channels[CHANNEL_NORMAL_B]->setPixel(x, y, cameraNormal.z());

            // Set final color in RGB buffer
            rChannel->setPixel(x, y, cameraNormal.x());
            gChannel->setPixel(x, y, cameraNormal.y());
            bChannel->setPixel(x, y, cameraNormal.z());
        }   
    }
}

void Framebuffer::render()
{
    for (auto const& [k, c] : m_channels)
    {
        c->allocate();
        c->clear();
    }

    m_channels[CHANNEL_Z]->fill(m_camera.getFarClip());

    //Pre-compute the view/projection only once per frame, rather than for every vertex
    m_view = m_camera.getViewMatrix();                          //View matrix
    m_proj = m_camera.getProjectionMatrix(m_width, m_height);   // Projection matrix

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

    PrintBuffer::debugPrintToScreen("Pixel count: %i", m_width * m_height);
}

void Framebuffer::allocateDisplayPtr()
{
    // Clear the color buffer
    if (m_displayBuffer)
    {
        VirtualFree(m_displayBuffer, 0, MEM_RELEASE);
        m_displayBuffer = nullptr;
    }

    // Calculate the new buffer size and allocate memory of that size
    int bufferSize = m_width * m_height * sizeof(unsigned int); // 4 bytes per pixel, display buffer is stored as unsigned int

    // Allocate the memory buffer
    m_displayBuffer = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    // Update the buffer BitmapInfo struct with the updated buffer size,
    // and width and height
    m_bufferBmi.bmiHeader.biSize = sizeof(m_bufferBmi.bmiHeader);
    m_bufferBmi.bmiHeader.biWidth = m_width;
    m_bufferBmi.bmiHeader.biHeight = -m_height; // When height is negative, it will invert the bitmap vertically.

    // Get the R, G, and B channels' pixel contents
    Channel* rChannel = m_channels[CHANNEL_R];
    Channel* gChannel = m_channels[CHANNEL_G];
    Channel* bChannel = m_channels[CHANNEL_B];

    Channel* nrChannel = m_channels[CHANNEL_NORMAL_R];
    Channel* ngChannel = m_channels[CHANNEL_NORMAL_G];
    Channel* nbChannel = m_channels[CHANNEL_NORMAL_B];

    uint8* row = (uint8*) m_displayBuffer;
    int pitch = m_width * sizeof(uint32);
    for (int y = 0; y < m_height; y++)
    {
        uint8* pixel = (uint8*) row;
        for (int x = 0; x < m_width; x++)
        {
            // Get r, g, b
            double r = rChannel->getPixel(x, y);
            double g = gChannel->getPixel(x, y);
            double b = bChannel->getPixel(x, y);

            // Normalize (0 => 255)
            // Order is inverted; RGB => BBGGRRXX
            if (pixel != NULL)
            {
                *pixel++ = (uint8) normalize(b, 0.0, 1.0, 0.0, 255.0);
                *pixel++ = (uint8) normalize(g, 0.0, 1.0, 0.0, 255.0);
                *pixel++ = (uint8) normalize(r, 0.0, 1.0, 0.0, 255.0);

                // A (alpha) is always 0
                *pixel++ = (uint8) 0;
            }
        }
        row += pitch;
    }
}

bool operator == (const Framebuffer& f1, const Framebuffer& f2)
{
    return typeid(f1) == typeid(f2);
}

MINI_NAMESPACE_CLOSE