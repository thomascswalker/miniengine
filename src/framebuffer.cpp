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

    // Create our grid points
    m_gridPoints.push_back(Vector3(-2.0, 0.0, -2.0));
    m_gridPoints.push_back(Vector3(-2.0, 0.0, 2.0));
    m_gridPoints.push_back(Vector3(2.0, 0.0, -2.0));
    m_gridPoints.push_back(Vector3(2.0, 0.0, 2.0));
}

Framebuffer::~Framebuffer()
{

}

HBITMAP Framebuffer::getBitmap()
{
    return CreateBitmap(m_width, m_height, 1, sizeof(double) * 4, m_displayBuffer);
}

void Framebuffer::bindTriangleBuffer(std::vector<Triangle*> data)
{
    m_triangles = data;
}

Vector3 Framebuffer::worldToScreen(Vector3* v)
{
    // Convert to normalized device coords
    Vector4 ndc = m_mvp * Vector4(*v, 1.0);          // m_mvp is precalculated in Framebuffer::render()

    double x = ((ndc._x + 1.0) * m_width) / 2.0;
    double y = ((ndc._y + 1.0) * m_height) / 2.0;

    v->_x = x;
    v->_y = y;
    v->_z = 1.0 / ndc._z;                          // Invert Z

    return Vector3(*v);
}

Vector3 Framebuffer::screenToWorld(double x, double y, double depth)
{
    // Screen to clip space
    double clipX = 2.0 * (x / (double) m_width) - 1.0;
    double clipY = 2.0 * (y / (double) m_height) - 1.0;
    double clipZ = 1.0 / depth;
    double clipW = 1.0;

    // Clip to camera space then world space (via MVP matrix)
    Matrix4 inverseMatrix = m_mvp.getInverse();
    Vector4 worldPosition = inverseMatrix * Vector4(clipX, clipY, clipZ, clipW);

    // Return final world position
    return Vector3(worldPosition._x, worldPosition._y, worldPosition._z);
}

double Framebuffer::getDepth(Vector3* v1, Vector3* v2, Vector3* v3, Vector3* p)
{
    // Calculate area of this triangle
    double a = area(*v1, *v2, *v3);

    // Calculate depth
    double w1 = area(*v2, *v3, *p);
    double w2 = area(*v3, *v1, *p);
    double w3 = area(*v1, *v2, *p);
    
    if (w1 < 0.0 && w2 < 0.0 && w3 < 0.0)
    {
        return DBL_MAX;
    }

    w1 /= a;
    w2 /= a;
    w3 /= a;

    return w1 * v1->_z + w2 * v2->_z + w3 * v3->_z;
}

Rect<int> Framebuffer::getBoundingBox(Vector3* v1, Vector3* v2)
{
    int x0 = std::min({v1->_x, v2->_x});
    int y0 = std::min({v1->_y, v2->_y});
    int x1 = std::max({v1->_x, v2->_x});
    int y1 = std::max({v1->_y, v2->_y});

    int width = x1 - x0;
    int height = y1 - y0;
    return Rect<int>(x0, y0, width, height);
}

Rect<int> Framebuffer::getBoundingBox(Vector3* v1, Vector3* v2, Vector3* v3)
{
    int x0 = std::min({v1->_x, v2->_x, v3->_x});
    int y0 = std::min({v1->_y, v2->_y, v3->_y});
    int x1 = std::max({v1->_x, v2->_x, v3->_x});
    int y1 = std::max({v1->_y, v2->_y, v3->_y});

    int width = x1 - x0;
    int height = y1 - y0;
    return Rect<int>(x0, y0, width, height);
}

bool Framebuffer::drawLine(Vector3* v1, Vector3* v2)
{
    Vector3 sv1 = worldToScreen(&*v1);
    Vector3 sv2 = worldToScreen(&*v2);

    Rect bounds = getBoundingBox(&sv1, &sv2);
    if (!m_frame.contains(bounds))
    {
        return false;
    }

    bounds.trim(m_frame);

    bool steep = false;

    double x0 = sv1._x;
    double y0 = sv1._y;
    double x1 = sv2._x;
    double y1 = sv2._y;

    if (std::abs(x0 - x1) < std::abs(y0 - y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    double dx = x1 - x0;
    double dy = y1 - y0;

    double e0 = std::abs(dy) * 2.0;
    double e1 = 0.0;

    int y = y0;
    for (int x = x0; x <= x1; x++)
    {
        int pixelOffset = 0;
        if (steep)
        {
            pixelOffset = y + (x * m_width);
        }
        else
        {
            pixelOffset = x + (y * m_width);
        }

        if (pixelOffset < 0 || pixelOffset > m_width * m_height)
        {
            continue;
        }
        getChannel(CHANNEL_R)->setPixel(pixelOffset, 0);
        getChannel(CHANNEL_G)->setPixel(pixelOffset, 0);
        getChannel(CHANNEL_B)->setPixel(pixelOffset, 255);

        e1 += e0;
        if (e1 > dx)
        {
            y += (y1 > y0 ? 1 : -1);
            e1 -= dx * 2;
        }
    }

    return true;
}

bool Framebuffer::drawCircle(Vector3* v, double r)
{
    int cx = (int) v->_x;
    int cy = (int) v->_y;

    // Top left
    int x0 = cx - r;
    int y0 = cy - r;

    // Bottom right
    int x1 = cx + r;
    int y1 = cy + r;

    // Clamp based on buffer width/height
    x0 = clamp(x0, 0, m_width);
    y0 = clamp(y0, 0, m_height);

    x1 = clamp(x1, 0, m_width);
    y1 = clamp(y1, 0, m_height);

    auto rsqr = pow(r, 2);

    for (int y = y0; y < y1; y++)
    {
        int rowOffset = y * m_width;

        for (int x = x0; x < x1; x++)
        {
            int pixelOffset = rowOffset + x;

            auto dx = x - cx;
            auto dy = y - cy;

            if (pow(dx, 2) + pow(dy, 2) <= rsqr)
            {
                getChannel(CHANNEL_R)->setPixel(pixelOffset, 255);
                getChannel(CHANNEL_G)->setPixel(pixelOffset, 0);
                getChannel(CHANNEL_B)->setPixel(pixelOffset, 0);
            }
        }
    }

    return true;
}

bool Framebuffer::drawTriangle(Triangle* triangle)
{
    // Construct a new shader for this triangle
    StandardShader* shader = new StandardShader();

    shader->width = m_width;
    shader->height = m_height;
    shader->matrix = m_mvp;
    shader->viewPosition = m_camera.getTranslation();

    // Get model-space vertex positions of the triangle
    Vector3 v1 = triangle->v1()->getTranslation();
    Vector3 v2 = triangle->v2()->getTranslation();
    Vector3 v3 = triangle->v3()->getTranslation();

    // Calculate world normal
    Vector3 worldNormal = getNormal(v1, v2, v3);
    worldNormal.normalize();
    shader->worldNormal = worldNormal;

    // Convert world normal to view normal
    Vector3 viewNormal = m_proj * m_view * worldNormal;
    viewNormal.normalize();
    shader->viewNormal = viewNormal;

    // Backface culling
    if (viewNormal._z < -1.0 || viewNormal._z > 1.0)
    {
        return false;
    }

    // Rescale from -1 => 1 to 0 => 1
    viewNormal.rescale(-1.0, 1.0, 0.0, 1.0);

    // Convert world-space to screenspace by running through the vertex shader
    shader->vertex(&v1);
    shader->vertex(&v2);
    shader->vertex(&v3);

    // Get the bounding box of the screen triangle
    // If the entire triangle is out of frame, skip
    // Clip vertices which are off screen
    Rect bounds = getBoundingBox(&v1, &v2, &v3);
    if (!m_frame.contains(bounds))
    {
        return false;
    }

    // Trim the size of the bounds rect to clip anything outside the frame.
    bounds.trim(m_frame);

    int x0 = bounds.getMin().x;
    int y0 = bounds.getMin().y;
    int x1 = bounds.getMax().x;
    int y1 = bounds.getMax().y;

    // Draw each pixel within the bounding box
    for (int y = y0; y < y1; y++)
    {
        int rowOffset = y * m_width;
        for (int x = x0; x < x1; x++)
        {
            // Current pixel index
            int pixelOffset = rowOffset + x;

            // If the pixel is outside the frame entirely, we'll skip it
            Vector3 pixelPosition(x + 1, y + 1, 0);
            shader->pixelPosition = pixelPosition;
            if (!m_frame.contains(pixelPosition._x, pixelPosition._y))
            {
                continue;
            }

            //Get barycentric coordinates of triangle (uvw)
            Vector3 uvw(0.0);
            if (!getBarycentricCoords(v1, v2, v3, pixelPosition, uvw))
            {
                // If the total != 1.0, or all of the coord axes are less than 0,
                // we'll skip this (it's not in the triangle!)
                continue;
            }
            shader->uvw = uvw;

            // Calculate depth
            double z = getDepth(&v1, &v2, &v3, &pixelPosition);

            // If the z-depth is greater (further back) than what's currently at this pixel, we'll
            // skip it. Also skip if we're outside of the near/far clip.
            double currentZ = getChannel(CHANNEL_Z)->getPixel(pixelOffset);
            if (z > currentZ || z < m_camera.getNearClip() || z > m_camera.getFarClip())
            {
                continue;
            }

            // Store z-depth in channel
            getChannel(CHANNEL_Z)->setPixel(pixelOffset, z);

            // Get world position from the current pixel, given the current depth
            shader->worldPosition = screenToWorld(x, y, z);

            // Compute fragment shader to get the final pixel color
            Vector3 finalColor = shader->fragment();

            // Set final color in RGB buffer
            getChannel(CHANNEL_R)->setPixel(pixelOffset, finalColor._x);
            getChannel(CHANNEL_G)->setPixel(pixelOffset, finalColor._y);
            getChannel(CHANNEL_B)->setPixel(pixelOffset, finalColor._z);
        }
    }

    return true;
}

void Framebuffer::render()
{
    m_channels[CHANNEL_R]->fill(0.0);
    m_channels[CHANNEL_G]->fill(0.0);
    m_channels[CHANNEL_B]->fill(0.0);

    // Reset z-buffer
    m_channels[CHANNEL_Z]->fill(m_camera.getFarClip());

    //Pre-compute the view/projection only once per frame, rather than for every vertex
    m_view = lookAt(m_camera.getTranslation(), m_camera.getTarget(), Vector3::up());  // View matrix
    m_proj = m_camera.getProjectionMatrix(m_width, m_height);                         // Projection matrix

    // Update MVP matrix
    m_mvp = m_proj * m_view;

    // Draw geometry
    int count = 0;
    for (auto t : m_triangles)
    {
        if (drawTriangle(t))
        {
            count++;
        }
        else
        {
            continue;
        }
    }
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
    auto rChannel = m_channels[CHANNEL_R];
    auto gChannel = m_channels[CHANNEL_G];
    auto bChannel = m_channels[CHANNEL_B];

    uint8* rowPtr = (uint8*) m_displayBuffer;
    int rowPtrOffset = m_width * sizeof(uint32);
    for (int y = 0; y < m_height; y++)
    {
        uint8* pixel = (uint8*) rowPtr;
        for (int x = 0; x < m_width; x++)
        {
            // Validate pixel is not null
            if (pixel == NULL)
            {
                continue;
            }

            // Pixel array offset
            int i = (m_width * y) + x;

            // Get r, g, b
            double r = rChannel->getPixel(i);
            double g = gChannel->getPixel(i);
            double b = bChannel->getPixel(i);

            // Normalize (0 => 255)
            // Order is inverted; RGB => BBGGRRXX
            *pixel++ = (uint8) rescale(&b, 0.0, 1.0, 0.0, 255.0);
            *pixel++ = (uint8) rescale(&g, 0.0, 1.0, 0.0, 255.0);
            *pixel++ = (uint8) rescale(&r, 0.0, 1.0, 0.0, 255.0);

            // A (alpha) is always 0
            *pixel++ = (uint8) 0;
        }
        rowPtr += rowPtrOffset;
    }
}

MINI_NAMESPACE_CLOSE