#ifndef MATH_H
#define MATH_H

#include <cstdlib>

#include "core.h"
#include "vector.h"

namespace Math
{
    int clamp(int *value, int min, int max)
    {
        if (*value < min)
        {
            return min;
        }
        if (*value > max)
        {
            return max;
        }
        return *value;
    }

    float area(Vector2 v1, Vector2 v2, Vector3 v3)
    {
        return abs((v1.x() * (v2.y() - v3.y()) + v2.x() * (v3.y() - v1.y()) + v3.x() * (v1.y() - v2.y())) / 2.0f);
    }

    float edge(Vector2 v1, Vector2 v2, Vector2 v3)
    {
        return (v1.x() - v3.x()) * (v2.y() - v3.y()) - (v2.x() - v3.x()) * (v1.y() - v3.y());
    }

    double distance(Vector2 v1, Vector2 v2)
    {
        auto a = pow(v2.y() - v1.y(), 2);
        auto b = pow(v2.x() - v1.x(), 2);
        return sqrt(a + b);
    }

    bool isPointInTriangle(Vector2 p, Vector2 v1, Vector2 v2, Vector2 v3)
    {
        // Get each vector
        auto ab = v1 - v2;
        auto ap = v1 - p;
        auto bc = v2 - v3;
        auto bp = v2 - p;
        auto ca = v3 - v1;
        auto cp = v3 - p;

        // Cross product of each edge/point
        auto d1 = ab.cross(ap);
        auto d2 = bc.cross(bp);
        auto d3 = ca.cross(cp);

        // Are either positive, or negative?
        bool isNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        bool isPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

        return !(isNeg && isPos);
    }

    void rotate(Vector3 v1, Vector3 v2, float rotation)
    {
        float values[12] = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 };
        Matrices::Matrix4 axis(values);
    }

    void rotate(Vector3 v, float rotation)
    {
        rotate(v, Vector3(), rotation);
    }
}

#endif