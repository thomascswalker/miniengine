#ifndef CORE_H
#define CORE_H

#ifndef MAIN_WINDOW_TIMER_ID
    #define MAIN_WINDOW_TIMER_ID 1001
#endif

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>
#include <tuple>
#include <chrono>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

namespace Core
{
    class Size
    {
    public:
        Size() {};
        Size(int width, int height) : m_width(width), m_height(height) {};

        int width() { return m_width; }
        int height() { return m_height; }
        int setWidth(int width) { m_width = width; }
        int setHeight(int height) { m_height = height; }

    private:
        int m_width = 0;
        int m_height = 0;
    };

    class Point
    {
    public:
        Point() {};
        Point(int x, int y) : m_x(x), m_y(y) {};

        int x() { return m_x; }
        int y() { return m_y; }
        int setX(int x) { m_x = x; }
        int setY(int y) { m_y = y; }

    private:
        int m_x = 0;
        int m_y = 0;
    };

    class Rect
    {
    public:
        // Constructors
        Rect() {};
        Rect(int x, int y, int width, int height) : m_x(x), m_y(y), m_width(width), m_height(height) {};

        // Methods
        Point pos() { return Point(m_x, m_y); }
        Size size() { return Size(m_width, m_height); }
        Point getMin() { return Point(m_x, m_y); }
        Point getMax() { return Point(m_x + m_width, m_y + m_height); }
    
        // Accessors
        int x() { return m_x; }
        int y() { return m_y; }
        int setX(int x) { m_x = x; }
        int setY(int y) { m_y = y; }
        int width() { return m_width; }
        int height() { return m_height; }
        int setWidth(int width) { m_width = width; }
        int setHeight(int height) { m_height = height; }

    private:
        // Properties
        int m_x = 0;
        int m_y = 0;
        int m_width = 0;
        int m_height = 0;
    };

    int getRefreshRate();
    Size getScreenSize();

    double time()
    {                
        auto duration = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        return (double) duration.count();
    };

    void print(const char* format, ...);
}


#endif
