#include "object.h"

void Object::move(const Vector3& v)
{
	Transform xForm = this->getTransform();
	xForm.setTranslation(getTranslation() + v);
	this->setTransform(xForm);
}