#include "object.h"

namespace Graphics {
using namespace Graphics;

void Object::move(const Vector3& v)
{
	m_transform.setTranslation(m_transform.getTranslation() + v);
}

}