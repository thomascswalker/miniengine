#ifndef CHANNEL_H
#define CHANNEL_H

#include "api.h"
#include "core.h"

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
    const char* m_name = "";
    
    int m_bufferSize = 0;
    int m_width = DEFAULT_WINDOW_WIDTH;
    int m_height = DEFAULT_WINDOW_HEIGHT;


 public:

    std::vector<double> m_pixels;

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
        size_t size = m_width * m_height;
        m_pixels.reserve(size);
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
        int size = getSize();
        for (int i = 0; i < size; i++)
        {
            m_pixels[i] = value;
        }
    };

    int getSize()
    {
        return m_width * m_height;
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

    double getPixel(int offset)
    {
        return m_pixels[offset];
    }

    double getPixel(int x, int y)
    {
        int offset = (m_width * y) + x;
        return m_pixels[offset];
    }

    void setPixel(int offset, double value)
    {
        m_pixels[offset] = value;
    }

    double setPixel(int x, int y, double value)
    {
        int offset = (m_width * y) + x;
        m_pixels[offset] = value;
    }
};

MINI_NAMESPACE_CLOSE

#endif