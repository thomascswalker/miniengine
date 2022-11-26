#ifndef MATH_H
#define MATH_H

#include <cstdlib>

#include "core.h"
#include "triangle.h"

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
inline T normalize(T* value,
                   const T amin,
                   const T amax,
                   const T bmin,
                   const T bmax)
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

/// <summary>
/// Calculate area of given triangle points.
/// </summary>
inline double area(Vector3& v1, Vector3& v2, Vector3& v3)
{
    double result = (v1._x * (v2._y - v3._y) +
                     v2._x * (v3._y - v1._y) +
                     v3._x * (v1._y - v2._y)) / 2.0;
    return result;
}

/// <summary>
/// 2D distance between two points.
/// </summary>
inline double distance(Vector2& v1, Vector2& v2)
{
    auto a = pow(v2._x - v1._x, 2);
    auto b = pow(v2._y - v1._y, 2);
    return sqrt(a + b);
}

/// <summary>
/// 3D distance between two points.
/// </summary>
inline double distance(Vector3& v1, Vector3& v2)
{
    auto a = pow(v2._x - v1._x, 2);
    auto b = pow(v2._y - v1._y, 2);
    auto c = pow(v2._z - v1._z, 2);
    return sqrt(a + b + c);
}

/// <summary>
/// Calculate normal vector given three triangle points.
/// </summary>
/// <param name="v1">First point of the triangle.</param>
/// <param name="v2">Second point of the triangle.</param>
/// <param name="v3">Third point of the triangle.</param>
/// <returns>The normal vector</returns>
inline Vector3 getNormal(Vector3& v1, Vector3& v2, Vector3& v3)
{
    Vector3 u = v2 - v1;
    Vector3 v = v3 - v1;
    return cross(u, v);
}

/// <summary>
/// Given a view normal, convert to the camera normal.
/// </summary>
/// <param name="viewNormal">The world normal * view matrix.</param>
/// <param name="forward">Forward component of the camera.</param>
/// <param name="up">Up component of the camera.</param>
/// <param name="right">Right component of the camera.</param>
/// <returns>The camera normal vector.</returns>
inline Vector3 getCameraNormal(Vector3& viewNormal,
                               Vector3& forward,
                               Vector3& up,
                               Vector3& right)
{
    // Calculate view direction
    forward.normalize();
    up.normalize();
    right.normalize();

    // Calculate facing, up, right ratios
    double facingRatio = dot(viewNormal, forward);
    double upRatio = dot(viewNormal, up);
    double rightRatio = dot(viewNormal, right);

    // Normalize ratios from -1 => 1 to 0 => 1
    facingRatio = normalize(&facingRatio, -1.0, 1.0, 0.0, 1.0);
    upRatio = normalize(&upRatio, -1.0, 1.0, 0.0, 1.0);
    rightRatio = normalize(&rightRatio, -1.0, 1.0, 0.0, 1.0);

    return Vector3(rightRatio, upRatio, facingRatio);
}

/// <summary>
/// Calculate barycentric coordinates for the given point 'p'.
/// </summary>
/// <param name="v1">First point in the triangle.</param>
/// <param name="v2">Second point in the triangle.</param>
/// <param name="v3">Third point in the triangle.</param>
/// <param name="p">The point to calculate barycentric coordinates for.</param>
/// <param name="uvw">The UVW vector to hold the calculated coordinates in.</param>
/// <returns>Returns true if point 'p' is in the triangle, false otherwise.</returns>
inline bool getBarycentricCoords(Vector3& v1,
                                 Vector3& v2,
                                 Vector3& v3,
                                 Vector3& p,
                                 Vector3& uvw)
{
    Vector3 v21 = v2 - v1;
    Vector3 v31 = v3 - v1;
    Vector3 vp1 = p - v1;

    double d00 = dot(v21, v21);
    double d01 = dot(v21, v31);
    double d11 = dot(v31, v31);
    double d20 = dot(vp1, v21);
    double d21 = dot(vp1, v31);

    double den = (d00 * d11) - (d01 * d01);

    uvw._y = (d11 * d20 - d01 * d21) / den;
    uvw._z = (d00 * d21 - d01 * d20) / den;
    uvw._x = 1.0 - uvw._y - uvw._z;

    double sum = uvw._x + uvw._y + uvw._z;
    double oneMinusSum = 1.0 - sum;
    bool isOutsideTriangle = uvw._x <= 0.0 || uvw._y <= 0.0 || uvw._z <= 0.0 || oneMinusSum > EPSILON;

    return isOutsideTriangle;
}


MINI_NAMESPACE_CLOSE

#endif