#ifndef MATH_H
#define MATH_H

#include <cstdlib>

#include "core.h"
#include "vector.h"
#include "matrix.h"

namespace Math
{
    constexpr auto PI = 3.14159;

    int clamp(int *value, int min, int max);

    float area(Vector2 v1, Vector2 v2, Vector3 v3);
    float edge(Vector2 v1, Vector2 v2, Vector2 v3);
    double distance(Vector2 v1, Vector2 v2);
    bool isPointInTriangle(Vector2 p, Vector2 v1, Vector2 v2, Vector2 v3);

    void rotate(Vector3 v1, Vector3 v2, float rotation);
    void rotate(Vector3 v, float rotation);
}

#endif