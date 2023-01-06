#ifndef STATICMESH_H
#define STATICMESH_H

#include "api.h"
#include "mesh.h"
#include "object.h"

namespace Graphics {
using namespace Graphics;

/// <summary>
/// Encapsulation class of raw mesh data so we can control one object's transformation while preserving
/// the original mesh's vertex positions.
/// </summary>
class StaticMesh :
	Object
{
	Mesh* m_mesh = new Mesh();

public:
	StaticMesh() { };
	StaticMesh(Mesh* mesh) :
		m_mesh(mesh) { };

	Mesh* getMesh() { return m_mesh; }
	void setMesh(Mesh* mesh) { m_mesh = mesh; }
};

}

#endif // !STATICMESH_H
