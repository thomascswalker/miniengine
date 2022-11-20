#ifndef MATH_H
#define MATH_H

#include <cstdlib>

#include "core.h"
#include "vector.h"

// Constants
#define     PI                          3.14159265359
#define     EPSILON                     DBL_EPSILON

// Trigonometry
#define     RADIANS(x)                  x * PI / 180.0
#define     DEGREES(x)                  x * 180.0 / PI

// Gamma
#define     INVERSE_GAMMA               1.0 / 2.2
#define     GAMMA_CORRECT(x)            255.0 * pow((x / 255.0), INVERSE_GAMMA)

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

/**
 * @brief Clamps value within the given min/max range.
 * @param value The value to clamp.
 * @param min The minimum value for the range.
 * @param max The maximum value for the range.
 * @return The clamped value.
*/
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

/**
 * @brief Convert value from Range A (old) to Range B (new)
 * @param value The value to convert.
 * @param amin Minimum value for Range A.
 * @param amax Maximum value for Range A.
 * @param bmin Minimum value for Range B.
 * @param bmax Maximum value for Range B.
 * @return The new normalized value.
*/
template <typename T>
inline T normalize(T* value, const T amin, const T amax, const T bmin, const T bmax)
{
    return (((*value - amin) * (bmax - bmin)) / (amax - amin)) + bmin;
}

/**
 * @brief 2D dot product.
*/
inline double dot(Vector2 v1, Vector2 v2)
{
    return v1._x * v2._x + v1._y * v2._y;
}

/**
 * @brief 3D dot product.
*/
inline double dot(Vector3 v1, Vector3 v2)
{
	return v1._x * v2._x + v1._y * v2._y + v1._z * v2._z;
}

/**
 * @brief 2D cross product.
*/
inline double cross(const Vector2& v1, const Vector2& v2)
{
	return v1._x * v2._y - v2._x * v1._y;
}

/**
 * @brief 3D cross product.
*/
inline Vector3 cross(const Vector3& v1, const Vector3& v2)
{
	double x = (v1._y * v2._z) - (v1._z * v2._y);
	double y = (v1._z * v2._x) - (v1._x * v2._z);
	double z = (v1._x * v2._y) - (v1._y * v2._x);
	return Vector3(x, y, z);
}

/**
 * @brief Calculate area of given triangle points.
 */
inline double area(Vector3* v1, Vector3* v2, Vector3* v3)
{
    return abs((v1->_x * (v2->_y - v3->_y) +
                v2->_x * (v3->_y - v1->_y) +
                v3->_x * (v1->_y - v2->_y)) / 2.0);
}

/**
 * @brief 2D distance between two points.
 */
inline double distance(Vector2 v1, Vector2 v2)
{
    auto a = pow(v2.x() - v1.x(), 2);
    auto b = pow(v2.y() - v1.y(), 2);
    return sqrt(a + b);
}

/**
 * @brief 3D distance between two points.
 */
inline double distance(Vector3 v1, Vector3 v2)
{
    auto a = pow(v2.x() - v1.x(), 2);
    auto b = pow(v2.y() - v1.y(), 2);
    auto c = pow(v2.z() - v1.z(), 2);
    return sqrt(a + b + c);
}

/**
 * @brief Calculate normal vector given three triangle points.
 * @return The normal vector.
*/
inline Vector3 getNormal(Vector3& v1, Vector3& v2, Vector3& v3)
{
    Vector3 u = v2 - v1;
    Vector3 v = v3 - v1;
    return cross(u, v);
}

/**
 * @brief Calculate barycentric coordinates for the given triangle points (v1, v2, v3) given
 * a point within the triangle (p).
 * @param v1 First point of the triangle.
 * @param v2 Second point of the triangle.
 * @param v3 Third point of the triangle.
 * @param p Target point of the triangle.
 * @param uvw Out vector containing the resulting UVW coordinates.
*/
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