#include "math.h"

namespace Math
{
    double Math::degreesToRadians(double d)
    {
        return d * (Math::PI / 180.0);
    }

    double Math::radiansToDegrees(double r)
    {
        return r * (180 / Math::PI);
    }

    float Math::dot(Vector3 v1, Vector3 v2)
    {
	    return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
    }

    int Math::clamp(int *value, int min, int max)
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

    float Math::area(Vector2 v1, Vector2 v2, Vector3 v3)
    {
        return abs((v1.x() * (v2.y() - v3.y()) + v2.x() * (v3.y() - v1.y()) + v3.x() * (v1.y() - v2.y())) / 2.0f);
    }

    float Math::edge(Vector2 v1, Vector2 v2, Vector2 v3)
    {
        return (v1.x() - v3.x()) * (v2.y() - v3.y()) - (v2.x() - v3.x()) * (v1.y() - v3.y());
    }

    double Math::distance(Vector2 v1, Vector2 v2)
    {
        auto a = pow(v2.y() - v1.y(), 2);
        auto b = pow(v2.x() - v1.x(), 2);
        return sqrt(a + b);
    }

    bool Math::isPointInTriangle(Vector2 p, Vector2 v1, Vector2 v2, Vector2 v3)
    {
        // Get each vector
        Vector2 ab = v1 - v2;
        Vector2 ap = v1 - p;
        Vector2 bc = v2 - v3;
        Vector2 bp = v2 - p;
        Vector2 ca = v3 - v1;
        Vector2 cp = v3 - p;

        // Cross product of each edge/point
        float d1 = cross(ab, ap);
        float d2 = cross(bc, bp);
        float d3 = cross(ca, cp);

        // Are either positive, or negative?
        bool isNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        bool isPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

        return !(isNeg && isPos);
    }
}