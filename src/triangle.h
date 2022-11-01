#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "maths.h"
#include "vertex.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

class Triangle
{
public:
	Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
		: m_v1(v1), m_v2(v2), m_v3(v3) { };

	Vertex v1() { return m_v1; }
	Vertex v2() { return m_v2; }
	Vertex v3() { return m_v3; }

	static Vector3 getNormal(Vector3& v1, Vector3& v2, Vector3& v3);
	static Vector3 getBarycentricCoords(Vector2& v1, Vector2& v2, Vector2& v3, Vector2& p);

private:
	Vertex m_v1;
	Vertex m_v2;
	Vertex m_v3;

	Vector3 normal;
};

MINI_NAMESPACE_CLOSE

#endif