#include "object.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

void Object::move(const Vector3& v)
{
	m_transform.setTranslation(m_transform.getTranslation() + v);
}

MINI_NAMESPACE_CLOSE