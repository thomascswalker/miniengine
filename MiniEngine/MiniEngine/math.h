#ifndef MATH_H
#define MATH_H

#include <cstdlib>

#include "core.h"
#include "vector.h"

namespace Math
{
    constexpr auto PI = 3.14159265359;
    constexpr auto EPSILON = 0.000000001;

    template <typename T>
    T clamp(T& value, T min, T max)
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

    double degreesToRadians(double d);
    double radiansToDegrees(double r);

    double dot(Vector3 v1, Vector3 v2);
    double area(Vector2 v1, Vector2 v2, Vector3 v3);
    double distance(Vector2 v1, Vector2 v2);
    double distance(Vector3 v1, Vector3 v2);
    bool isPointInTriangle(Vector2 p, Vector2 v1, Vector2 v2, Vector2 v3);
}

#endif