#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "maths.h"
#include "vertex.h"

namespace Graphics {
using namespace Graphics;

class Triangle
{
public:
	Triangle(Vertex* v1, Vertex* v2, Vertex* v3)
		: m_v1(v1),
		  m_v2(v2),
		  m_v3(v3) { };

	Vertex* v1() { return m_v1; }
	Vertex* v2() { return m_v2; }
	Vertex* v3() { return m_v3; }

private:
	Vertex* m_v1;
	Vertex* m_v2;
	Vertex* m_v3;

	Vector3 normal;
};

}

#endif