#ifndef TRIANGLE_H
#define TRIANGLE_H
#endif

#include "vertex.h"

class Triangle
{
public:
	Triangle() : m_v1(Vertex()), m_v2(Vertex()), m_v3(Vertex()) {};

	Vertex v1() { return m_v1; }
	Vertex v2() { return m_v2; }
	Vertex v3() { return m_v3; }

private:
	Vertex m_v1;
	Vertex m_v2;
	Vertex m_v3;
};
