#include "mesh.h"

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
    switch (space)
    {
        default:
        case Coordinates::Local:
        {
            return m_vertices;
        }
        case Coordinates::World:
        {
            std::vector<Vertex> worldVertices;

            for (Vertex localVertex : m_vertices)
            {
                // Create a new world-space vertex
                Vertex worldVertex = Vertex();

                // Set the world position via position
                auto localPos = localVertex.getTranslation();
                auto worldPos = getPosition();
                worldVertex.setTranslation(localPos + worldPos);

                // Set the world position via rotation

                // Add this world-space vertex to the worldVertices list
                worldVertices.push_back(worldVertex);
            }

            return worldVertices;
        }
    }
}
