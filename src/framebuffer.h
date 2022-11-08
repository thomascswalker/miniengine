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


class Framebuffer
{
public:
    Framebuffer(HWND hwnd);
    ~Framebuffer();
    
    // Parameters
    int getWidth() { return m_width; }                         
    void setWidth(int width) { m_width = width; }
    int getHeight() { return m_height; }
    void setHeight(int height) { m_height = height; }

    void setSize(Size size);

    template <typename T>
    inline void setSize(T width, T height)
    {
        m_width = (int) width;
        m_height = (int) height;

        for (auto const& [k, c] : m_channels)
        {
            c->setSize((int) width, (int) height);
            c->allocate();
            c->clear();
        }
    }

    // Camera
    Camera* camera() { return &m_camera; }

    // Pixel buffer
    HWND getHwnd() { return m_hwnd; }
    BITMAPINFO* getBitmapInfo() { return &m_bufferBmi; }

    void allocateDisplayPtr();
    void* getDisplayPtr() { return m_displayBuffer; }
    int getBufferSize() { return m_width * m_height * sizeof(unsigned int); }

    // Vertex, index, triangle buffer
    void bindVertexBuffer(std::vector<Vertex> data);
    void bindIndexBuffer(std::vector<int> data);
    void bindTriangleBuffer(std::vector<Triangle> data);
    size_t getNumVertices();
    size_t getNumIndices();
    size_t getNumTriangles();

    // Math
    bool worldToScreen(Vector3& v);
    bool isPointInFrame(Vector2& p) const;
    bool isRectInFrame(Rect& r) const;
    double getDepth(Vector3& v1, Vector3& v2, Vector3& v3, Vector3& p);

    // Drawing
    void drawTriangle(Vector3& v1, Vector3& v2, Vector3& v3);
    Rect getBoundingBox(Vector3& v1, Vector3& v2, Vector3& v3);
    void render();

    // Matrices
    Vector3 getTargetTranslation() { return m_targetPosition; }

    void setModelMatrix(Matrix4& m) { m_model = m; }
    Matrix4 getViewMatrix() { return m_view; }
    Matrix4 getProjectionMatrix() { return m_proj; }
    Matrix4 getModelViewProjMatrix() { return m_mvp; }

    double modelRotation = 0.0;

protected:
    friend bool operator == (const Framebuffer&,
                             const Framebuffer&);

private:
    // Window handle
    const HWND m_hwnd = HWND();

    int m_width = 512;
    int m_height = 512;

    // Channels
    std::map<const char*, Channel*> m_channels;

    // Pixel memory
    SIZE_T m_bufferSize = 0;
    void* m_displayBuffer = nullptr;
    BITMAPINFO m_bufferBmi; 
    const int m_bytesPerPixel = 4;

    // Vertex memory
    std::vector<Vertex> m_vertices;
    std::vector<int> m_indices;
    std::vector<Triangle> m_triangles;

    std::vector<Vector2> m_screenVertices;
    std::vector<double> m_depthBuffer;

    // Camera and matrices
    Camera m_camera;
    Vector3 m_targetPosition;
    Matrix4 m_view = Matrix4();
    Matrix4 m_proj = Matrix4();
    Matrix4 m_mvp = Matrix4();
    Matrix4 m_model = Matrix4();
};

MINI_NAMESPACE_CLOSE

#endif