#ifndef MATH_H
#define MATH_H

#include <cstdlib>

#include "core.h"
#include "vector.h"

// Constants
#define     PI                          3.14159265359
#define     EPSILON                     0.000000001

// Trigonometry
#define     RADIANS(x)                  x * PI / 180.0
#define     DEGREES(x)                  x * 180.0 / PI

// Gamma
#define     INVERSE_GAMMA               1.0 / 2.2
#define     GAMMA_CORRECT(x)            255.0 * pow((x / 255.0), INVERSE_GAMMA)

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

template <typename T>
inline T clamp(T& value, T min, T max)
{
    if (value < min)
    {
        return min;
    }
    if (value > max)
    {
        return max;
    }
    return value;
};

// Convert from Range A (old) to Range B (new)
template <typename T>
inline T normalize(T value, T amin, T amax, T bmin, T bmax)
{
    return (((value - amin) * (bmax - bmin)) / (amax - amin)) + bmin;
}

template <typename T>
inline T normalizeNew(T* value, const T amin, const T amax, const T bmin, const T bmax)
{
    return (((*value - amin) * (bmax - bmin)) / (amax - amin)) + bmin;
}

inline double dot(Vector2 v1, Vector2 v2)
{
    return v1.x() * v2.x() + v1.y() * v2.y();
}

inline double dot(Vector3 v1, Vector3 v2)
{
	return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

inline double area(Vector2 v1, Vector2 v2, Vector3 v3)
{
    return abs((v1.x() * (v2.y() - v3.y()) + v2.x() * (v3.y() - v1.y()) + v3.x() * (v1.y() - v2.y())) / 2.0);
}

inline double distance(Vector2 v1, Vector2 v2)
{
    auto a = pow(v2.x() - v1.x(), 2);
    auto b = pow(v2.y() - v1.y(), 2);
    return sqrt(a + b);
}

inline double distance(Vector3 v1, Vector3 v2)
{
    auto a = pow(v2.x() - v1.x(), 2);
    auto b = pow(v2.y() - v1.y(), 2);
    auto c = pow(v2.z() - v1.z(), 2);
    return sqrt(a + b + c);
}


MINI_NAMESPACE_CLOSE

#endif