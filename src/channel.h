#ifndef CHANNEL_H
#define CHANNEL_H

#include "api.h"
#include "core.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

/// Macros for channel names
#define CHANNEL_PIXEL_SIZE  sizeof(double)
#define CHANNEL_R           "R"
#define CHANNEL_G           "G"
#define CHANNEL_B           "B"
#define CHANNEL_Z           "Z"
#define CHANNEL_NORMAL_R    "Normal_R"
#define CHANNEL_NORMAL_G    "Normal_G"
#define CHANNEL_NORMAL_B    "Normal_B"

/// Array of default channel names
static const char* const CHANNELS[] = {
    CHANNEL_R,
    CHANNEL_G,
    CHANNEL_B,
    CHANNEL_Z,
    CHANNEL_NORMAL_R,
    CHANNEL_NORMAL_G,
    CHANNEL_NORMAL_B
};

/// <summary>
/// Channels are comprised of an array of doubles. Values in the array are meant to be
/// accessed or modified based on the current width/height of the channel.
/// </summary>
class Channel
{
    /// The channel name
    const char* m_name = "";
    
    int m_bufferSize = 0;
    int m_width = DEFAULT_WINDOW_WIDTH;
    int m_height = DEFAULT_WINDOW_HEIGHT;

 public:
    /// Pixel array
    std::vector<double> m_pixels;

    /// <summary>
    /// Default constructor.
    /// </summary>
    /// <param name="name">The name of the channel.</param>
    /// <param name="width">Starting width of the channel.</param>
    /// <param name="height">Starting width of the channel.</param>
    Channel(const char* name, int width, int height)
        : m_name(name),
          m_width(width),
          m_height(height)
    {
        allocate();
        clear();
    };
    ~Channel() {};

    /// <summary>
    /// Reserves the space in the pixel array for the current size of the channel frame
    /// (width * height).
    /// </summary>
    void allocate()
    {
        size_t size = (size_t) m_width * (size_t) m_height;
        m_pixels.reserve(size);
    };

    /// <summary>
    /// Set all pixel values to 0.
    /// </summary>
    void clear()
    {
        fill(0.0);
    };

    /// <summary>
    /// Fills the pixel array with the given value.
    /// </summary>
    /// <param name="value">The value to set all pixels to.</param>
    void fill(double value)
    {
        int size = getSize();
        for (int i = 0; i < size; i++)
        {
            m_pixels[i] = value;
        }
    };

    /// <summary>
    /// Returns the size of the current channel frame.
    /// </summary>
    int getSize()
    {
        return m_width * m_height;
    }

    /// <summary>
    /// Set the size of the current channel frame. This will also re-allocate
    /// the memory for this channel and clear it (set all pixel values to 0).
    /// </summary>
    /// <param name="width">The new width.</param>
    /// <param name="height">The new height.</param>
    void setSize(int width, int height)
    {
        m_width = width;
        m_height = height;
        allocate();
        clear();
    }

    /// <summary>
    /// Get the pixel value at the position in the pixel array.
    /// </summary>
    /// <param name="offset">The index in the pixel array to retrieve the value from.</param>
    /// <returns>The value of the pixel at the given offset.</returns>
    double getPixel(int offset)
    {
        return m_pixels[offset];
    }

    double getPixel(int x, int y)
    {
        int offset = (m_width * y) + x;
        return m_pixels[offset];
    }

    /// <summary>
    /// Set the pixel value at the position in the pixel array.
    /// </summary>
    /// <param name="offset">The index in the pixel array set the value to..</param>
    /// <param name="value">The value to set at the position.</param>
    void setPixel(int offset, double value)
    {
        m_pixels[offset] = clamp(value, 0.0, 1.0);
    }

    double setPixel(int x, int y, double value)
    {
        int offset = (m_width * y) + x;
        m_pixels[offset] = value;
    }
};

MINI_NAMESPACE_CLOSE

#endif