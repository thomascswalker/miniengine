#ifndef OBJECT_H
#define OBJECT_H

#include "transform.h"

namespace Graphics {
using namespace Graphics;

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

	void setTranslation(const Vector3& t) { m_transform.setTranslation(t); }
	void setRotation(const Rotation& r) { m_transform.setRotation(r); }

	void addTranslation(const Vector3& t)
	{
		Vector3 newTranslation = m_transform.getTranslation() + t;
		m_transform.setTranslation(newTranslation);
	}
	void addRotation(const Rotation& r)
	{
		Rotation newRotation = m_transform.getRotation() + r;
		m_transform.setRotation(newRotation);
	}

	void move(const Vector3& v);

protected:
	Transform m_transform = Transform();
};

}

#endif