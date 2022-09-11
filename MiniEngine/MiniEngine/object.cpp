#include "object.h"

void Object::move(const Vector3& v)
{
	Transform xForm = getTransform();
	xForm.setTranslation(xForm.getTranslation() + v);
	setTransform(xForm);
}