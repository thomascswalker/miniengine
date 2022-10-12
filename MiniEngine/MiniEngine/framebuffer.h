#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define NOMINMAX

#include <memory>
#include <typeinfo>
#include <windows.h>
#include <cassert>
#include <sstream>

#include "camera.h"
#include "color.h"
#include "matrix.h"
#include "mesh.h"

enum Buffer
{
    RGB,
    DEPTH,
    NORMAL
};

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
    void setSize(Core::Size size);
    void setSize(int width, int height);

    // Camera
    Camera* camera() { return &m_camera; }

    // Pixel buffer
    HWND getHwnd() { return m_hwnd; }
    void clear();
    void allocate();
    BITMAPINFO* getBitmapInfo() { return &m_bufferBmi; }
    void* getMemoryPtr() { return (unsigned int*)m_memoryBuffer; }
    int getBufferSize() { return m_width * m_height * sizeof(unsigned int); }

    // Vertex, index, triangle buffer
    void bindVertexBuffer(std::vector<Vertex> data);
    void bindIndexBuffer(std::vector<int> data);
    void bindTriangleBuffer(std::vector<Triangle> data);
    size_t getNumVertices();
    size_t getNumIndices();
    size_t getNumTriangles();

    // Math
    Vector3 worldToScreen(Vector3& v);
    bool isPointInFrame(Vector2& p) const;

    // Drawing
    void setPixel(int x, int y, Color color, Buffer buffer = Buffer::RGB);
    void setPixel(Vector2& v, Color color, Buffer buffer = Buffer::RGB);
    void drawRect(int x0, int y0, int x1, int y1, Color color);
    void drawCircle(int cx, int cy, int r, Color color);
    void drawCircle(Vector2& v, int r, Color color);
    void drawLine(Vector2& v1, Vector2& v2, Color color);
    void drawTriangle(Vector3& v1, Vector3& v2, Vector3& v3);
    void render();

protected:
    friend bool operator == (const Framebuffer&,
                             const Framebuffer&);

private:
    // Window handle
    const HWND m_hwnd = HWND();

    int m_width = 640;
    int m_height = 480;

    // Pixel memory
    SIZE_T m_bufferSize;
    void* m_memoryBuffer;
    void* m_depthBuffer;
    BITMAPINFO m_bufferBmi; 
    const int m_bytesPerPixel = 4;
    int m_rowLength = 0;

    // Vertex memory
    //void* m_vertexBuffer;
    std::vector<Vertex> m_vertices;

    //void* m_indexBuffer;
    std::vector<int> m_indices;

    //void* m_triangleBuffer;
    std::vector<Triangle> m_triangles;

    int m_stride    = 12; // Should be 32 with X, Y, Z, R, G, B, U, V
    int m_posOffset = 0;
    int m_colOffset = 12;
    int m_texOffset = 24;

    std::vector<Vector2> m_screenVertices;

    // Camera and matrices
    Camera m_camera;
};

#endif