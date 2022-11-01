#include "object.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

void Object::move(const Vector3& v)
{
	Transform xForm = getTransform();
	xForm.setTranslation(getTranslation() + v);
	this->setTransform(xForm);
}

MINI_NAMESPACE_CLOSE