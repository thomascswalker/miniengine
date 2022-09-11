#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"

class Transform
{
public:
	Transform() {};

	Transform&		identity();

	void			setMatrix(const Matrix4& m);
	Matrix4			getMatrix() const;

	void			setTranslation(const Vector3& t);
	const Vector3&	getTranslation() { return m_translation; }
	void			setRotation(const Rotation& r);
	const Rotation& getRotation() { return m_rotation; }
	void			setScale(const Vector3& s);
	const Vector3&	getScale() { return m_scale; }


private:
	Vector3 m_translation;
	Rotation m_rotation;
	Vector3 m_scale;

	Rotation m_pivotOrientation;
	Vector3 m_pivotPosition;
};

#endif