#include "mesh.h"

void Mesh::addTri(Vertex v1, Vertex v2, Vertex v3)
{
    m_vertices.push_back(v1);
    m_vertices.push_back(v2);
    m_vertices.push_back(v3);
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
                auto localPos = localVertex.pos();
                auto worldPos = getPosition();
                worldVertex.setPos(localPos + worldPos);

                // Set the world position via rotation

                // Add this world-space vertex to the worldVertices list
                worldVertices.push_back(worldVertex);
            }

            return worldVertices;
        }
    }
}
