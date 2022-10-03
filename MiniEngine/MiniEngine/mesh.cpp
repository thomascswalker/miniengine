#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<int> indices)
    : m_vertices(vertices), m_indices(indices)
{
    for (int i = 0; i < indices.size(); i++)
    {
        int i1 = indices[i]; i++;
        int i2 = indices[i]; i++;
        int i3 = indices[i];

        auto v1 = vertices[i1];
        auto v2 = vertices[i2];
        auto v3 = vertices[i3];

        addTri(v1, v2, v3);
    }
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

std::vector<Vertex> Mesh::getVertices(Coordinates::CoordSpace space)
{
    return m_vertices;
}
