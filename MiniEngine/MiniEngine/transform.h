#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"

class Transform
{
public:
	Transform() {};

	Transform& identity();

	void setMatrix(const Matrix4& m);
	Matrix4 getMatrix() const;

	void setTranslation(const Vector3& t);
	void setRotation(const Rotation& r);
	void setScale(const Vector3& s);


private:
	Vector3 m_translation;
	Rotation m_rotation;
	Vector3 m_scale;

	Rotation m_pivotOrientation;
	Vector3 m_pivotPosition;
};

#endif