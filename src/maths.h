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

namespace Math
{
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

    // Convert from Range A (old) to Range B (new)
    template <typename T>
    T normalize(T value, T amin, T amax, T bmin, T bmax)
    {
        return (((value - amin) * (bmax - bmin)) / (amax - amin)) + bmin;
    }

    template <typename T>
    T normalizeNew(T* value, const T amin, const T amax, const T bmin, const T bmax)
    {
        return (((*value - amin) * (bmax - bmin)) / (amax - amin)) + bmin;
    }
    double dot(Vector2 v1, Vector2 v2);
    double dot(Vector3 v1, Vector3 v2);
    double area(Vector2 v1, Vector2 v2, Vector3 v3);
    double distance(Vector2 v1, Vector2 v2);
    double distance(Vector3 v1, Vector3 v2);
    double edge(Vector3& v1, Vector3& v2, Vector3& v3);
}

#endif