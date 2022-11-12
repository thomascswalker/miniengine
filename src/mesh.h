#ifndef MESH_H
#define MESH_H

#include <vector>

#include "coordinates.h"
#include "triangle.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

class Mesh
{
public:
	Mesh() {};
	Mesh(std::vector<Vertex> vertices, std::vector<int> indices);

	void addTri(Vertex* v1, Vertex* v2, Vertex* v3);
	void addTri(Triangle* t);
	size_t numVertices();

	std::vector<Vertex> getVertices() { return m_vertices;}
	std::vector<Vertex> getVertices(CoordSpace space);
	std::vector<int> getIndices() { return m_indices; }
	std::vector<Triangle*> getTris() { return m_triangles; }

	void setVertices(const std::vector<Vertex> data);
	void setIndices(const std::vector<int> data);
	void bindTris();

private:
	std::vector<Vertex> m_vertices;
	std::vector<int> m_indices;

	std::vector<Triangle*> m_triangles;
};

MINI_NAMESPACE_CLOSE

#endif