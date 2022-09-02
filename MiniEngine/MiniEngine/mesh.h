#ifndef MESH_H
#define MESH_H

#include <vector>

#include "coordinates.h"
#include "vertex.h"

class Mesh
{
public:
	Mesh() {};
	Mesh(std::vector<Vertex> vertices, std::vector<int> indices) : m_vertices(vertices), m_indices(indices) {};

	void addTri(Vertex v1, Vertex v2, Vertex v3);
	size_t numVertices();

	std::vector<Vertex> getVertices() { return m_vertices;}
	std::vector<Vertex> getVertices(Coordinates::CoordSpace space);
	std::vector<int> getIndices() { return m_indices; }

	Vector3 getPosition() { return m_position; }
	void setPosition(Vector3 position) { m_position = position; }
	void addPosition(Vector3 position) { m_position += position; }

	Vector3 getRotation() { return m_rotation; }
	void setRotation(Vector3 rotation) { m_rotation = rotation; }
	void addRotation(Vector3 rotation) { m_rotation += rotation; }

	Vector3 getScale() { return m_scale; }
	void setScale(Vector3 scale) { m_scale = scale; }
	void addScale(Vector3 scale) { m_scale += scale; }

private:
	std::vector<Vertex> m_vertices;
	std::vector<int> m_indices;

	Vector3 m_position = {0, 0, 0};
	Vector3 m_rotation = {0, 0, 0};
	Vector3 m_scale	   = {1, 1, 1};
};

#endif