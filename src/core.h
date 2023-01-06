#ifndef CORE_H
#define CORE_H

#ifndef MAIN_WINDOW_TIMER_ID
    #define MAIN_WINDOW_TIMER_ID 1001
#endif

#include <windows.h>
#include <windowsx.h>
#include <cstdio>
#include <cstdarg>
#include <cctype>
#include <ctime>
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

namespace Graphics {
using namespace Graphics;

class Size
{
public:
    Size() = default;;
    Size(double _width, double _height)
        : width(_width), height(_height) {};

    double width = 0;
    double height = 0;
};

template <typename T>
class Point
{
public:
    Point() = default;;
    Point(T _x, T _y)
        : x(_x), y(_y) {};

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
    Rect()
        : x(0), y(0), width(1), height(1) {};
    Rect(T _x, T _y, T _width, T _height)
        : x(_x), y(_y), width(_width), height(_height) {};
    Rect(Vector2 min, Vector2 max)
        : x(min._x), y(min._y), width(max._x - min._x), height(max._y - min._y) {};

    // Methods
    Point<T> pos() { return Point(x, y); }
    Size size() { return Size(width, height); }
    Point<T> getMin() { return Point(x, y); }
    Point<T> getMax() { return Point(x + width, y + height); }

    inline bool overlaps(T _x, T _y)
    {
        return (_x > x ||
                _x < x + width - 1 ||
                _y > y ||
                _y < y + height - 1);
    }

    inline bool overlaps(Rect r)
    {
        return (r.x > x ||
                r.x + r.width < x + width ||
                r.y > y ||
                r.y + r.height < y + height);
    }

    inline bool contains(T _x, T _y)
    {
        return (_x > x &&
                _x < x + width &&
                _y > y &&
                _y < y + height);
    }

    inline bool contains(Rect r)
    {
        return (r.x > x &&
                r.x + r.width < x + width &&
                r.y > y &&
                r.y + r.height < y + height);
    }

    /// <summary>
    /// Trim this rectangle with the bounds of the given rectangle.
    /// </summary>
    /// <param name="r">The rectangle that does the trimming.</param>
    inline void trim(Rect r)
    {
        if (x < r.x)
        {
            x = r.x;
        }
        if (y < r.y)
        {
            y = r.y;
        }
        if (width > r.width)
        {
            width = r.width;
        }
        if (height > r.height)
        {
            height = r.height;
        }
    }

    inline std::string toString()
    {
        int mx = x + width;
        int my = y + height;

        std::string string = "[";
        string += std::to_string(x);
        string += ",";
        string += std::to_string(y);
        string += ",";
        string += std::to_string(mx);
        string += ",";
        string += std::to_string(my);
        string += "]";
        return string;
    }

    // Properties
    T x;
    T y;
    T width;
    T height;
};

int getRefreshRate();
Size getScreenSize();

double getCurrentTime();
int getDeltaTime(int previous, int offset);

void print(const char* format, ...);

}

#endif
