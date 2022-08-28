#include "mesh.h"

void Mesh::addTri(Vertex v1, Vertex v2, Vertex v3)
{
    m_vertices.push_back(v1);
    m_vertices.push_back(v2);
    m_vertices.push_back(v3);
}

std::vector<Vertex> Mesh::getVertices(Coordinates::CoordSpace space)
{
    switch (space)
    {
        case Coordinates::Local:
        {
            return m_vertices;
        }
        case Coordinates::World:
        {
            std::vector<Vertex> worldVertices;

            for (Vertex localVertex : m_vertices)
            {
                Vertex worldVertex = Vertex();
                auto localPos = localVertex.pos();
                auto worldPos = getPosition();
                worldVertex.setPos(localPos + worldPos);
                worldVertices.push_back(worldVertex);
            }

            return worldVertices;
        }
    }
}
