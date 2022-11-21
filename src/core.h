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
    Size(double _width, double _height) : width(_width), height(_height) {};

    double width = 0;
    double height = 0;
};

template <typename T>
class Point
{
public:
    Point() {};
    Point(T _x, T _y) : x(_x), y(_y) {};

    //T x() { return m_x; }
    //T y() { return m_y; }
    //void setX(T x) { m_x = x; }
    //void setY(T y) { m_y = y; }

    bool operator == (Point& p) const
    {
        return x == p.x && y == p.y;
    }

    bool operator != (Point& p) const
    {
        return x != p.x && y != p.y;
    }

    T x = 0;
    T y = 0;
};

class Rect
{
public:
    // Constructors
    Rect() {};
    Rect(double _x, double _y, double _width, double _height) : x(_x), y(_y), width(_width), height(_height) {};
    Rect(Vector2 min, Vector2 max);


    // Methods
    Point<double> pos() { return Point(x, y); }
    Size size() { return Size(width, height); }
    Point<double> getMin() { return Point(x, y); }
    Point<double> getMax() { return Point(x + width, y + height); }

    /**
     * @brief Check whether the rectangle contains the given X/Y coordinates.
     * @param _x The x-coordinate.
     * @param _y The y-coordinate.
     * @return Whether the rectangle contains the X/Y coordinates or not.
    */
    inline bool contains(int _x, int _y)
    {
        if (_x < x || _x > width)
        {
            return false;
        }
        if (_y < y || _y > height)
        {
            return false;
        }
        return true;
    }

    /**
     * @brief Check whether the rectangle contains the given point.
     * @param p The point to check.
     * @return Whether the rectangle contains the point or not.
    */
    inline bool contains(Point<int> p)
    {
        return contains(p.x, p.y);
    }

    /**
     * @brief Check whether the rectangle contains the given vector.
     * @param v The vector to check.
     * @return Whether the rectangle contains the vector or not.
    */
    inline bool contains(Vector3& v)
    {
        return contains((int) v._x, (int) v._y);
    }

    // Properties
    double x = 0.0;
    double y = 0.0;
    double width = 0.0;
    double height = 0.0;
};

int getRefreshRate();
Size getScreenSize();

double getCurrentTime();
int getDeltaTime(int previous, int offset);

void print(const char* format, ...);

MINI_NAMESPACE_CLOSE

#endif
