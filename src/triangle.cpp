#include "triangle.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

// https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal#:~:text=A%20surface%20normal%20for%20a,of%20the%20face%20w.r.t.%20winding).
Vector3 Triangle::getNormal(Vector3& v1, Vector3& v2, Vector3& v3)
{
    Vector3 u = v2 - v1;
    Vector3 v = v3 - v1;
    return cross(u, v);
}

Vector3 Triangle::getBarycentricCoords(Vector2& v1, Vector2& v2, Vector2& v3, Vector2& p)
{
    Vector2 v21 = v2 - v1;
    Vector2 v31 = v3 - v1;
    Vector2 vp1 = p - v1;

    double d00 = dot(v21, v21);
    double d01 = dot(v21, v31);
    double d11 = dot(v31, v31);
    double d20 = dot(vp1, v21);
    double d21 = dot(vp1, v31);

    double det = (d00 * d11) - (d01 * d01);

    double v = (d11 * d20 - d01 * d21) / det;
    double w = (d00 * d21 - d01 * d20) / det;
    double u = 1.0 - v - w;

    return Vector3(u, v, w);
}

MINI_NAMESPACE_CLOSE