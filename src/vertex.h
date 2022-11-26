#ifndef VERTEX_H
#define VERTEX_H

#include "vector.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

class Vertex
{
public:
	Vertex() : m_translation(Vector3()) {};
	Vertex(double x, double y, double z) : m_translation(Vector3(x, y, z)) {};
	Vertex(Vector3& v) : m_translation(v) {};
	Vertex(const Vector3& v) : m_translation(v) {};

	Vector3 getTranslation() { return m_translation; }
	void setTranslation(const Vector3& t) { m_translation = t; }

	static const int stride		 = 32;
	static const int posOffset	 = 0;
	static const int colorOffset = 12;
	static const int uvOffset	 = 24;

private:
	Vector3 m_translation;
};

MINI_NAMESPACE_CLOSE

#endif