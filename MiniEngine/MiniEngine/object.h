#ifndef OBJECT_H
#define OBJECT_H

#include "transform.h"

class Object
{
public:
	Object() {};

	const Transform getTransform() { return m_transform; }
	void setTransform(Transform t) { m_transform = t; }

private:
	Transform m_transform = Transform();
};

#endif