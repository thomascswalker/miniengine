#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define NOMINMAX

#include <memory>
#include <typeinfo>
#include <windows.h>
#include <cassert>
#include <sstream>
#include <map>

#include "camera.h"
#include "channel.h"
#include "color.h"
#include "matrix.h"
#include "mesh.h"
#include "printbuffer.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

/**
 * @brief Main class for managing displaying to the screen.
*/
class Framebuffer
{
    // Window handle
    const HWND m_hwnd = HWND();

    int m_width = DEFAULT_WINDOW_WIDTH;
    int m_height = DEFAULT_WINDOW_HEIGHT;
    Rect m_frame;

    // Channels
    std::map<const char*, Channel*> m_channels;

    // Pixel memory
    SIZE_T m_bufferSize = 0;
    void* m_displayBuffer = nullptr;
    BITMAPINFO m_bufferBmi; 
    const int m_bytesPerPixel = 4;

    // Vertex memory
    std::vector<Triangle*> m_triangles;

    // Camera and matrices
    Camera m_camera;
    Vector3 m_targetPosition;
    Matrix4 m_view = Matrix4();
    Matrix4 m_proj = Matrix4();
    Matrix4 m_mvp = Matrix4();
    Matrix4 m_model = Matrix4();

public:
    // Constructor
    Framebuffer(HWND hwnd);

    // Destructor
    ~Framebuffer();
    
    int getWidth() { return m_width; }                         
    void setWidth(int width) { m_width = width; }
    int getHeight() { return m_height; }
    void setHeight(int height) { m_height = height; }

    /**
     * @brief Set the Framebuffer to the given width and height. This also sets all channels
     * to this new size.
     * @param width The new width.
     * @param height The new height.
    */
    template <typename T>
    inline void setSize(T width, T height)
    {
        m_width = (int) width;
        m_height = (int) height;
        m_frame = Rect(0, 0, width, height);

        for (auto const& [k, c] : m_channels)
        {
            c->setSize((int) width, (int) height);
            c->allocate();
            c->clear();
        }
    }

    inline void setSize(Size size) { setSize(size.width, size.height); }

    /**
     * @brief Returns the channel pointer from the given channel name.
     * @param channel The channel name. See definitions in "channel.h".
     * @return The pointer to the Channel object.
    */
    inline Channel* getChannel(const char* channel) { return m_channels[channel]; }

    // Camera
    Camera* camera() { return &m_camera; }

    // Pixel buffer
    HWND getHwnd() { return m_hwnd; }

    /**
     * @brief Allocates the display memory void pointer by combining the
     * R, G, and B channels into a single void pointer filled with uint8 for
     * each channel, with the fourth chunk set to zero.
     * 
     * @brief
     * Because channel pixels are doubles, we need to convert those to 0 => 255.
     * 
     * @brief
     *  R  |  G  |  B  | None 
     * --- | --- | --- | ---- 
     * 255 | 128 | 50  | 0
    */
    void allocateDisplayPtr();
    void* getDisplayPtr() { return m_displayBuffer; }
    int getBufferSize() { return m_width * m_height * sizeof(unsigned int); }

    /**
     * @brief Generates a HBITMAP based on the current content of the display buffer.
     * @return The HBITMAP struct object generated from the display buffer.
    */
    HBITMAP getBitmap();

    /**
     * @brief Set the buffer of triangles to the given triangle list.
     * @param data The pointer list of triangles to bind.
    */
    void bindTriangleBuffer(std::vector<Triangle*> data);

    /**
     * @brief Given a point, determine if it fits within the current Framebuffer frame.
     * @param p The point to check.
     * @return Whether the point is within the frame.
    */
    bool isPointInFrame(Vector2& p) const;

    /**
     * @brief Given a rectangle, determine if it fits within the current Framebuffer frame.
     * @param r The rectangle to check.
     * @return Whether the rectangle is within the frame.
    */
    bool isRectInFrame(Rect& r) const;

    /**
     * @brief Converts the given world-space vector to a screen-space vector. This
     * takes into account our MVP matrix is already generated in Framebuffer::render().
     * @param v The world-space vector to convert.
     * @return Whether the conversion worked or not.
    */
    bool worldToScreen(Vector3& v);

    /**
     * @brief Returns the Z-depth of point P given a triangle (v1, v2, v3).
     * @param v1 Screen-space position of vertex 1.
     * @param v2 Screen-space position of vertex 2.
     * @param v3 Screen-space position of vertex 3.
     * @param p Screen-space position of target point to find depth of.
     * @return The depth value of the point.
    */
    double getDepth(Vector3* v1, Vector3* v2, Vector3* v3, Vector3* p);

    /**
     * @brief Returns the 2D bounding box of the screen-space points provided.
     * @param v1 Screen-space position of vertex 1.
     * @param v2 Screen-space position of vertex 2.
     * @param v3 Screen-space position of vertex 3.
     * @return The bounding box of the triangle.
    */
    Rect getBoundingBox(Vector3& v1, Vector3& v2, Vector3& v3);

    /**
     * @brief Converts the world-space vertex positions to normalized screen-space positions. 
     * This will allocate the R, G, and B channels to contain the rendered pixel data.
     * @param v1 World-space position of vertex 1.
     * @param v2 World-space position of vertex 2.
     * @param v3 World-space position of vertex 3.
     * @return Whether the triangle was drawn or not.
    */
    bool drawTriangle(Vector3& v1, Vector3& v2, Vector3& v3);

    /**
     * @brief Clears the current list of channels, calculates the new MVP camera matrix,
     * and, for each triangle which is bound, renders that triangle.
    */
    void render();

    // Matrices
    Vector3 getTargetTranslation() { return m_targetPosition; }

    void setModelMatrix(Matrix4& m) { m_model = m; }
    Matrix4 getViewMatrix() { return m_view; }
    Matrix4 getProjectionMatrix() { return m_proj; }
    Matrix4 getModelViewProjMatrix() { return m_mvp; }

    double modelRotation = 0.0;
};

MINI_NAMESPACE_CLOSE

#endif