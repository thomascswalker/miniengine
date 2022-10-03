#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vertex.h"

class Triangle
{
public:
	Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
		: m_v1(v1), m_v2(v2), m_v3(v3) { };

private:
	Vertex m_v1;
	Vertex m_v2;
	Vertex m_v3;

	Vector3 normal;
};


#endif