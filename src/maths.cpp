#include "maths.h"

using namespace Math;

double Math::dot(Vector2 v1, Vector2 v2)
{
    return v1.x() * v2.x() + v1.y() * v2.y();
}

double Math::dot(Vector3 v1, Vector3 v2)
{
	return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

double Math::area(Vector2 v1, Vector2 v2, Vector3 v3)
{
    return abs((v1.x() * (v2.y() - v3.y()) + v2.x() * (v3.y() - v1.y()) + v3.x() * (v1.y() - v2.y())) / 2.0);
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

double Math::edge(Vector3& v1, Vector3& v2, Vector3& v3)
{
    return (v3.x() - v1.x()) * (v2.y() - v1.y()) - (v3.y() - v1.y()) * (v2.x() - v1.x());
}
