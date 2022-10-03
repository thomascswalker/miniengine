#include "math.h"

using namespace Math;

double Math::degreesToRadians(double d)
{
    return d * (PI / 180.0);
}

double Math::radiansToDegrees(double r)
{
    return r * (180 / PI);
}

double Math::dot(Vector3 v1, Vector3 v2)
{
	return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

double Math::area(Vector2 v1, Vector2 v2, Vector3 v3)
{
    return abs((v1.x() * (v2.y() - v3.y()) + v2.x() * (v3.y() - v1.y()) + v3.x() * (v1.y() - v2.y())) / 2.0f);
}

double Math::distance(Vector2 v1, Vector2 v2)
{
    auto a = pow(v2.x() - v1.x(), 2);
    auto b = pow(v2.y() - v1.y(), 2);
    return sqrt(a + b);
}

double Math::distance(Vector3 v1, Vector3 v2)
{
    auto a = pow(v2.x() - v1.x(), 2);
    auto b = pow(v2.y() - v1.y(), 2);
    auto c = pow(v2.z() - v1.z(), 2);
    return sqrt(a + b + c);
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
    double d1 = cross(ab, ap);
    double d2 = cross(bc, bp);
    double d3 = cross(ca, cp);

    // Are either positive, or negative?
    bool isNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool isPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(isNeg && isPos);
}
