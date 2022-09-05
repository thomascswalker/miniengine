#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <memory>
#include <typeinfo>
#include <windows.h>
#include <gdiplus.h>
#include <cassert>
#include <sstream>

#include "color.h"
#include "matrix.h"
#include "math.h"
#include "mesh.h"
#include "vertex.h"

enum Buffer
{
    RGB,
    DEPTH
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

    // Pixel buffer
    HWND getHwnd() { return m_hwnd; }
    void allocate();
    BITMAPINFO* getBitmapInfo() { return &m_bufferBmi; }
    void* getMemoryPtr() { return (unsigned int*)m_colorBuffer; }
    int getBufferSize() { return m_width * m_height * sizeof(unsigned int); }

    // Vertex buffer
    void setVertexBufferData(std::vector<Vertex> data);
    void setIndexBufferData(std::vector<int> data);
    int getNumVertices();
    int getNumIndices();

    // Math
    Vector2 worldToScreen(Vector3 vector, Matrices::Matrix4 matrix);

    // Drawing
    void clear();
    void setPixel(int x, int y, Color color, Buffer buffer = Buffer::RGB);
    void drawRect(int x0, int y0, int x1, int y1, Color color);
    void drawCircle(int cx, int cy, int r, Color color);
    void drawTri(Vector2& v1, Vector2& v2, Vector2& v3, Color color);
    void drawLine(Vector2& v1, Vector2& v2, Color color);
    void drawScene(Matrices::Matrix4 m, bool bDrawFaces, bool bDrawEdges, bool bDrawVertices);
    void drawGradient();

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
    void* m_colorBuffer;
    void* m_depthBuffer;
    BITMAPINFO m_bufferBmi; 
    const int m_bytesPerPixel = 4;
    int m_rowLength = 0;

    // Vertex memory
    void* m_vertexBuffer;
    std::vector<Vertex> m_vertices;
    int m_numVertices;
    void* m_indexBuffer;
    std::vector<int> m_indices;
    int m_numIndices;
    int m_stride    = 12; // Should be 32 with X, Y, Z, R, G, B, U, V
    int m_posOffset = 0;
    int m_colOffset = 12;
    int m_texOffset = 24;
};

#endif