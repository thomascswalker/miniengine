#include "mesh.h"

void Mesh::addTri(Vertex v1, Vertex v2, Vertex v3)
{
    m_vertices.push_back(v1);
    m_vertices.push_back(v2);
    m_vertices.push_back(v3);
}