#include "triangle.h"

Vector3
Triangle::getBarycentricCoords(Vector2& v1, Vector2& v2, Vector2& v3, Vector2& p)
{
    Vector2 v21 = v2 - v1;
    Vector2 v31 = v3 - v1;
    Vector2 vp1 = p - v1;

    double d00 = Math::dot(v21, v21);
    double d01 = Math::dot(v21, v31);
    double d11 = Math::dot(v31, v31);
    double d20 = Math::dot(vp1, v21);
    double d21 = Math::dot(vp1, v31);

    double det = (d00 * d11) - (d01 * d01);

    double v = (d11 * d20 - d01 * d21) / det;
    double w = (d00 * d21 - d01 * d20) / det;
    double u = 1.0 - v - w;

    return Vector3(u, v, w);
}