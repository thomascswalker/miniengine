#include "mesh.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<int> indices)
    : m_vertices(vertices), m_indices(indices)
{
    bindTris();
}

void Mesh::addTri(Vertex v1, Vertex v2, Vertex v3)
{
    auto t = Triangle(v1, v2, v3);
    addTri(t);
}

void Mesh::addTri(Triangle t)
{
    m_triangles.push_back(t);
}

size_t Mesh::numVertices()
{
    return m_vertices.size();
}

std::vector<Vertex> Mesh::getVertices(CoordSpace space)
{
    return m_vertices;
}

void Mesh::setVertices(const std::vector<Vertex> data)
{
    m_vertices = data;
}

void Mesh::setIndices(const std::vector<int> data)
{
    m_indices = data;
}

void Mesh::bindTris()
{
    for (int i = 0; i < m_indices.size(); i++)
    {
        int i1 = m_indices[i]; i++;
        int i2 = m_indices[i]; i++;
        int i3 = m_indices[i];

        Vertex* v1 = &m_vertices[i1];
        Vertex* v2 = &m_vertices[i2];
        Vertex* v3 = &m_vertices[i3];

        addTri(*v1, *v2, *v3);
    }
}

MINI_NAMESPACE_CLOSE