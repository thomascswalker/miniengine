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
    
}

HBITMAP Framebuffer::getBitmap()
{
    return CreateBitmap(m_width, m_height, 1, sizeof(double) * 4, m_displayBuffer);
}

void Framebuffer::bindTriangleBuffer(std::vector<Triangle*> data)
{
    m_triangles = data;
}

bool Framebuffer::worldToScreen(Vector3* v)
{
    // Convert to normalized device coords
    Vector4 ndc = m_mvp * Vector4(*v, 1.0);          // m_mvp is precalculated in Framebuffer::render()

    double x = ((ndc._x + 1.0) * m_width) / 2.0;
    double y = ((ndc._y + 1.0) * m_height) / 2.0;

    v->_x = x;
    v->_y = y;
    v->_z = 1.0 / ndc._z;                          // Invert Z

    return true;
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

bool Framebuffer::drawTriangle(Triangle* worldTriangle)
{
    // Get model-space vertex positions of the triangle
    Vector3 v1 = m_model * worldTriangle->v1()->getTranslation();
    Vector3 v2 = m_model * worldTriangle->v2()->getTranslation();
    Vector3 v3 = m_model * worldTriangle->v3()->getTranslation();

    // Calculate world normal
    Vector3 worldNormal = getNormal(v1, v2, v3);
    worldNormal.normalize();

    // Convert to camera space
    Vector3 viewNormal = m_view * worldNormal;

    // Calculate the camera normal
    Vector3 forward = -m_camera.getForward();
    Vector3 up = -m_camera.getUp();
    Vector3 right = -m_camera.getRight();
    Vector3 cameraNormal = getCameraNormal(viewNormal, forward, up, right);
    //cameraNormal.normalize();

    // Backface cull if enabled
#ifdef BACKFACE_CULL
    if (cameraNormal._z < 0.0)
    {
        return false;
    }
#endif

    // Convert world-space to screenspace
    worldToScreen(&v1);
    worldToScreen(&v2);
    worldToScreen(&v3);

    // Get the bounding box of the screen triangle
    // If the entire triangle is out of frame, skip
    // Clip vertices which are off screen
    Rect bounds = getBoundingBox(&v1, &v2, &v3);

    // TODO: This should be overlap, but overlap fails on backside tris
    // OR contains tris which should be clipped or are too big for whatever
    // reason. Perhaps tris need to be clipped after being determined they're
    // overlapping?
    if (!m_frame.contains(bounds))
    {
        return false;
    }

    // Trim the size of the bounds rect to clip anything outside the frame.
    bounds.trim(m_frame);

    int minX = bounds.getMin().x;
    int minY = bounds.getMin().y;
    int maxX = bounds.getMax().x;
    int maxY = bounds.getMax().y;

    // Draw each pixel within the bounding box
    for (int y = minY; y < maxY; y++)
    {
        int rowOffset = y * m_width;
        for (int x = minX; x < maxX; x++)
        {
            // Current pixel index
            int pixelOffset = rowOffset + x;

            // If the pixel is outside the frame entirely, we'll skip it
            Vector3 p(x + 1.0, y + 1.0, 0);
            if (!m_frame.contains(x, y))
            {
                continue;
            }

            //Get barycentric coordinates of triangle (uvw)
            Vector3 uvw(0.0);
            if (!getBarycentricCoords(v1, v2, v3, p, uvw))
            {
                // If the total != 1.0, or all of the coord axes are less than 0,
                // we'll skip this (it's not in the triangle!)
                continue;
            }

            // Calculate depth
            double z = getDepth(&v1, &v2, &v3, &p);

            // If the z-depth is greater (further back) than what's currently at this pixel, we'll
            // skip it. Also skip if we're outside of the near/far clip.
            double currentZ = getChannel(CHANNEL_Z)->getPixel(pixelOffset);
            if (z > currentZ || z < m_camera.getNearClip() || z > m_camera.getFarClip())
            {
                continue;
            }

            // Store z-depth in channel
            getChannel(CHANNEL_Z)->setPixel(pixelOffset, z); // Otherwise we'll set the current pixel Z to this depth

            // Set final color in RGB buffer
            getChannel(CHANNEL_R)->setPixel(pixelOffset, cameraNormal._x);
            getChannel(CHANNEL_G)->setPixel(pixelOffset, cameraNormal._y);
            getChannel(CHANNEL_B)->setPixel(pixelOffset, cameraNormal._z);
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
    m_mvp = m_proj * m_view * m_model;

    int count = 0;
    for (auto t : m_triangles)
    {
        if (drawTriangle(t))
        {
            count++;
        }
    }
    PrintBuffer::debugPrintToScreen("Visible tris: %i", count);
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
            *pixel++ = (uint8) normalize(&b, 0.0, 1.0, 0.0, 255.0);
            *pixel++ = (uint8) normalize(&g, 0.0, 1.0, 0.0, 255.0);
            *pixel++ = (uint8) normalize(&r, 0.0, 1.0, 0.0, 255.0);

            // A (alpha) is always 0
            *pixel++ = (uint8) 0;
        }
        rowPtr += rowPtrOffset;
    }
}

MINI_NAMESPACE_CLOSE