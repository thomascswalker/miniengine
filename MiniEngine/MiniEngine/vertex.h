#ifndef VERTEX_H
#define VERTEX_H

#include "vector.h"

class Vertex
{
public:
	Vertex() : m_pos(Vector3()) {};
	Vertex(float x, float y, float z) : m_pos(Vector3(x, y, z)) {};
	Vertex(Vector3 vec) : m_pos(vec) {};

	Vector3 pos() { return m_pos; }
	void setPos(Vector3 pos) { m_pos = pos; }

	static const int stride		 = 32;
	static const int posOffset	 = 0;
	static const int colorOffset = 12;
	static const int uvOffset	 = 24;

private:
	Vector3 m_pos;
};

#endif