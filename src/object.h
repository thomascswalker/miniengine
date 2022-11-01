#ifndef OBJECT_H
#define OBJECT_H

#include "transform.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

class Object
{
public:
	Object() {};

	const Transform getTransform() { return m_transform; }
	void setTransform(Transform t) { m_transform = t; }

	const Vector3 getTranslation() { return m_transform.getTranslation(); }
	const Rotation getRotation() { return m_transform.getRotation(); }
	const Vector3 getScale() { return m_transform.getScale(); }

	const Vector3 getForward() { return m_transform.getForward(); }
	const Vector3 getRight() { return m_transform.getRight(); }
	const Vector3 getUp() { return m_transform.getUp(); }

	void move(const Vector3& v);

private:
	Transform m_transform = Transform();
};

MINI_NAMESPACE_CLOSE

#endif