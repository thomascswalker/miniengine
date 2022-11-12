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

#include "printbuffer.h"
#include "vector.h"

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

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

class Size
{
public:
    Size() {};
    Size(double width, double height) : m_width(width), m_height(height) {};

    double width() { return m_width; }
    double height() { return m_height; }
    double setWidth(int width) { m_width = width; }
    double setHeight(int height) { m_height = height; }

private:
    double m_width = 0;
    double m_height = 0;
};

template <typename T>
class Point
{
public:
    Point() {};
    Point(T x, T y) : m_x(x), m_y(y) {};

    T x() { return m_x; }
    T y() { return m_y; }
    void setX(T x) { m_x = x; }
    void setY(T y) { m_y = y; }

    bool operator == (Point& p) const
    {
        return m_x == p.x() && m_y == p.y();
    }

    bool operator != (Point& p) const
    {
        return m_x != p.x() && m_y != p.y();
    }

private:
    T m_x = 0;
    T m_y = 0;
};

class Rect
{
public:
    // Constructors
    Rect() {};
    Rect(double x, double y, double width, double height) : m_x(x), m_y(y), m_width(width), m_height(height) {};
    Rect(Vector2 min, Vector2 max);


    // Methods
    Point<double> pos() { return Point(m_x, m_y); }
    Size size() { return Size(m_width, m_height); }
    Point<double> getMin() { return Point(m_x, m_y); }
    Point<double> getMax() { return Point(m_x + m_width, m_y + m_height); }
    
    // Accessors
    double x() { return m_x; }
    double y() { return m_y; }
    double setX(int x) { m_x = x; }
    double setY(int y) { m_y = y; }
    double width() { return m_width; }
    double height() { return m_height; }
    double setWidth(double width) { m_width = width; }
    double setHeight(double height) { m_height = height; }

    inline bool contains(int x, int y)
    {
        if (x < m_x || x > m_width)
        {
            return false;
        }
        if (y < m_y || y > m_height)
        {
            return false;
        }
        return true;
    }
    inline bool contains(Point<int> p)
    {
        return contains(p.x(), p.y());
    }
    inline bool contains(Vector3& v)
    {
        return contains((int) v.x(), (int) v.y());
    }

private:
    // Properties
    double m_x = 0;
    double m_y = 0;
    double m_width = 0;
    double m_height = 0;
};

int getRefreshRate();
Size getScreenSize();

double getCurrentTime();

void print(const char* format, ...);

MINI_NAMESPACE_CLOSE

#endif
