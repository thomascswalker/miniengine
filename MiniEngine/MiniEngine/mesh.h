#ifndef MESH_H
#define MESH_H
#endif

#include <vector>
#include "vertex.h"

class Mesh
{
public:
	Mesh() {};

	void addTri(Vertex v1, Vertex v2, Vertex v3);
	int numVertices() { return m_vertices.size(); }

private:
	std::vector<Vertex> m_vertices;
	std::vector<int> m_indices;
};
