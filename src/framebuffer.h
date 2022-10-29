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
#include "printbuffer.h"

MINI_USING_DIRECTIVE

#define CHANNEL_PIXEL_SIZE sizeof(double)
#define CHANNEL_R 0
#define CHANNEL_G 1
#define CHANNEL_B 2
#define CHANNEL_Z 3

enum BufferType
{
    RGB,
    DEPTH,
    NORMAL
};

/*
Channels are comprised of an array of doubles.
*/
class Channel
{
 public:
    Channel(const char* name, int width, int height)
        : m_name(name),
          m_width(width),
          m_height(height)
    {
        allocate();
        clear();
    };
    ~Channel() {};

    void allocate()
    {
        // Clear the color buffer
        if (m_memoryBuffer)
        {
            VirtualFree(m_memoryBuffer, 0, MEM_RELEASE);
            m_memoryBuffer = nullptr;
        }

        // Calculate the new buffer size and allocate memory of that size
        m_bufferSize = m_width * m_height * sizeof(double);

        // Allocate the memory buffer
        m_memoryBuffer = VirtualAlloc(0, m_bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    };

    /*
    Set all pixel values to 0.
    */
    void clear()
    {
        fill(0.0);
    };

    void fill(double value)
    {
        for (int x = 0; x < m_width; x++)
        {
            for (int y = 0; y < m_height; y++)
            {
                setPixel(x, y, value);
            }
        }
    };

    int size()
    {
        return m_width * m_height * sizeof(double);
    }

    void setSize(int width, int height)
    {
        m_width = width;
        m_height = height;
        allocate();
        clear();
    }

    int getMemoryOffset(int x, int y)
    {
        return ((y * m_width) + x) * 8;
    }

    double getPixel(int x, int y)
    {
        int offset = getMemoryOffset(x, y);
        double* buffer = (double*) m_memoryBuffer;
        buffer += offset;
        return *buffer;
    }

    std::vector<double> getPixels()
    {
        auto buffer = (double*) m_memoryBuffer;
        std::vector<double> pixels;
        for (int i = 0; i < size(); i++)
        {
            auto value = *buffer;
            pixels.push_back(value);
            buffer++;
        }
        return pixels;
    }

    void setPixel(int x, int y, double value)
    {
        int offset = getMemoryOffset(x, y);
        double* memoryPtr = (double*) m_memoryBuffer;
        *memoryPtr = value;
    }

private:
    const char* m_name = "";
    void* m_memoryBuffer = nullptr;
    int m_bufferSize = 0;
    int m_width = 0;
    int m_height = 0;

    std::vector<double> m_pixels;
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
    void setSize(Size size);
    void setSize(int width, int height);

    // Camera
    Camera* camera() { return &m_camera; }



    // Pixel buffer
    HWND getHwnd() { return m_hwnd; }
    BITMAPINFO* getBitmapInfo() { return &m_bufferBmi; }

    void allocateDisplayPtr();
    void* getDisplayPtr() { return m_displayBuffer; }


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
    bool isRectInFrame(Rect& r) const;
    double getDepth(Vector3& v1, Vector3& v2, Vector3& v3, Vector3& p);

    // Drawing
    void setPixel(int x, int y, Color color, BufferType buffer = BufferType::RGB);
    void setPixel(Vector2& v, Color color, BufferType buffer = BufferType::RGB);
    void drawRect(int x0, int y0, int x1, int y1, Color color);
    void drawCircle(int cx, int cy, int r, Color color);
    void drawCircle(Vector2& v, int r, Color color);
    void drawLine(Vector2& v1, Vector2& v2, Color color);
    void drawTriangle(Vector3& v1, Vector3& v2, Vector3& v3);
    Rect getBoundingBox(Vector3& v1, Vector3& v2, Vector3& v3);
    void render();

    // Matrices
    Vector3 getTargetTranslation() { return m_targetPosition; }
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

    int m_width = 640;
    int m_height = 480;

    // Channels
    std::vector<Channel> m_channels;

    // Pixel memory
    SIZE_T m_bufferSize;
    void* m_displayBuffer;
    void* m_memoryBuffer;
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

#endif