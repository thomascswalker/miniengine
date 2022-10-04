#include "triangle.h"

Vector3
Triangle::getBarycentricCoords(Vector2& v1, Vector2& v2, Vector2& v3, Vector2& p)
{
    Vector2 sv0 = v2 - v1;
    Vector2 sv1 = v3 - v1;
    Vector2 sv2 = p - v1;

    double d00 = Math::dot(sv0, sv0);
    double d01 = Math::dot(sv0, sv1);
    double d11 = Math::dot(sv1, sv1);
    double d20 = Math::dot(sv2, sv0);
    double d21 = Math::dot(sv2, sv1);

    double det = (d00 * d11) - (d01 * d01);

    double v = (d11 * d20 - d01 * d21) / det;
    double w = (d00 * d21 - d01 * d20) / det;
    double u = 1.0f - v - w;

    return Vector3(u, v, w);
}