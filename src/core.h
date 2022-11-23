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

template <typename T>
class Rect
{
public:
    // Constructors
    Rect() {};
    Rect(T _x, T _y, T _width, T _height)
        : x(_x), y(_y), width(_width), height(_height) {};
    Rect(Vector2 min, Vector2 max)
        : x(min._x), y(min._y), width(max._x - min._x), height(max._y - min._y) {};

    // Methods
    Point<T> pos() { return Point(x, y); }
    Size size() { return Size(width, height); }
    Point<T> getMin() { return Point(x, y); }
    Point<T> getMax() { return Point(x + width, y + height); }

    /**
     * @brief Check whether the rectangle contains the given X/Y coordinates.
     * @param _x The x-coordinate.
     * @param _y The y-coordinate.
     * @return Whether the rectangle contains the X/Y coordinates or not.
    */
    inline bool contains(T _x, T _y)
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
    inline bool contains(Point<T> p)
    {
        return contains(p.x, p.y);
    }

    inline bool contains(Rect r)
    {
        return contains(r.getMin()) && contains(r.getMin());
    }

    /**
     * @brief Check whether the rectangle contains the given vector.
     * @param v The vector to check.
     * @return Whether the rectangle contains the vector or not.
    */
    inline bool contains(Vector3& v)
    {
        return contains(v._x, v._y);
    }

    // Properties
    T x = 0.0;
    T y = 0.0;
    T width = 0.0;
    T height = 0.0;
};

int getRefreshRate();
Size getScreenSize();

double getCurrentTime();
int getDeltaTime(int previous, int offset);

void print(const char* format, ...);

MINI_NAMESPACE_CLOSE

#endif
