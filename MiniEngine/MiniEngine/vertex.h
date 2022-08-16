#ifndef VERTEX_H
#define VERTEX_H
#endif

#include "vector.h"

class Vertex
{
public:
	Vertex() : m_pos(Vector3()) {};

	Vector3 pos() { return m_pos; }
	void setPos(Vector3 pos) { m_pos = pos; }

private:
	Vector3 m_pos;
};
