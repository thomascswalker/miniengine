#ifndef OBJECT_H
#define OBJECT_H

#include "transform.h"

class Object
{
public:
	Object() {};

	const Transform getTransform() { return m_transform; }
	void setTransform(Transform t) { m_transform = t; }

	const Vector3 getTranslation() { return m_transform.getTranslation(); }
	const Rotation getRotation() { return m_transform.getRotation(); }
	const Vector3 getScale() { return m_transform.getScale(); }

	void move(const Vector3& v);

private:
	Transform m_transform = Transform();
};

#endif