#ifndef STATICMESH_H
#define STATICMESH_H

#include "api.h"
#include "mesh.h"
#include "object.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

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

MINI_NAMESPACE_CLOSE

#endif // !STATICMESH_H
