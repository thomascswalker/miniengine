#ifndef CHANNEL_H
#define CHANNEL_H

#include "api.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

#define CHANNEL_PIXEL_SIZE  sizeof(double)
#define CHANNEL_R           "R"
#define CHANNEL_G           "G"
#define CHANNEL_B           "B"
#define CHANNEL_Z           "Z"
#define CHANNEL_NORMAL_R    "Normal_R"
#define CHANNEL_NORMAL_G    "Normal_G"
#define CHANNEL_NORMAL_B    "Normal_B"

static const char* const CHANNELS[] = {
    CHANNEL_R,
    CHANNEL_G,
    CHANNEL_B,
    CHANNEL_Z,
    CHANNEL_NORMAL_R,
    CHANNEL_NORMAL_G,
    CHANNEL_NORMAL_B
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
        // Each raw channel is stored as double to maximize floating point precision without
        // getting too huge
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
        for (int y = 0; y < m_height; y++)
        {
            double* memoryPtr = (double*) m_memoryBuffer;
            memoryPtr += y * m_width;
            for (int x = 0; x < m_width; x++)
            {
                *memoryPtr++ = value;
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
        return x + (y * m_width);
    }

    template <typename T>
    double getPixel(T x, T y)
    {
        int offset = getMemoryOffset((int)x, (int)y);
        double* buffer = (double*) m_memoryBuffer;
        buffer += offset;
        return *buffer;
    }

    template <typename T>
    void setPixel(T x, T y, double value)
    {
        uint32 offset = getMemoryOffset((int)x, (int)y);
        double* buffer = (double*) m_memoryBuffer;
        buffer += offset;
        *buffer = value;
    }

    void pasteBuffer(void* buffer)
    {
        memcpy(m_memoryBuffer, buffer, size());
    }

private:
    const char* m_name = "";
    void* m_memoryBuffer = nullptr;
    int m_bufferSize = 0;
    int m_width = 0;
    int m_height = 0;

    std::vector<double> m_pixels;
};

MINI_NAMESPACE_CLOSE

#endif