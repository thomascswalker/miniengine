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
inline T normalize(T* value, const T amin, const T amax, const T bmin, const T bmax)
{
    return (((*value - amin) * (bmax - bmin)) / (amax - amin)) + bmin;
}

inline double dot(Vector2 v1, Vector2 v2)
{
    return v1._x * v2._x + v1._y * v2._y;
}

inline double dot(Vector3 v1, Vector3 v2)
{
	return v1._x * v2._x + v1._y * v2._y + v1._z * v2._z;
}

inline double area(Vector2* v1, Vector2* v2, Vector3* v3)
{
    return abs((v1->_x * (v2->_y - v3->_y) +
                v2->_x * (v3->_y - v1->_y) +
                v3->_x * (v1->_y - v2->_y)) / 2.0);
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

inline Vector3 getNormal(Vector3& v1, Vector3& v2, Vector3& v3)
{
    Vector3 u = v2 - v1;
    Vector3 v = v3 - v1;
    return cross(u, v);
}

inline void getBarycentricCoords(Vector2& v1, Vector2& v2, Vector2& v3, Vector2& p, Vector3& uvw)
{
    Vector2 v21 = v2 - v1;
    Vector2 v31 = v3 - v1;
    Vector2 vp1 = p - v1;

    double d00 = dot(v21, v21);
    double d01 = dot(v21, v31);
    double d11 = dot(v31, v31);
    double d20 = dot(vp1, v21);
    double d21 = dot(vp1, v31);

    double den = (d00 * d11) - (d01 * d01);

    uvw._y = (d11 * d20 - d01 * d21) / den;
    uvw._z = (d00 * d21 - d01 * d20) / den;
    uvw._x = 1.0 - uvw._y - uvw._z;
}


MINI_NAMESPACE_CLOSE

#endif