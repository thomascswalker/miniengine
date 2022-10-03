#ifndef MESH_H
#define MESH_H

#include <vector>

#include "coordinates.h"
#include "triangle.h"

class Mesh
{
public:
	Mesh() {};
	Mesh(std::vector<Vertex> vertices, std::vector<int> indices);

	void addTri(Vertex v1, Vertex v2, Vertex v3);
	void addTri(Triangle t);
	size_t numVertices();

	std::vector<Vertex> getVertices() { return m_vertices;}
	std::vector<Vertex> getVertices(Coordinates::CoordSpace space);
	std::vector<int> getIndices() { return m_indices; }
	std::vector<Triangle> getTris() { return m_triangles; }

private:
	std::vector<Vertex> m_vertices;
	std::vector<int> m_indices;

	std::vector<Triangle> m_triangles;
};

#endif