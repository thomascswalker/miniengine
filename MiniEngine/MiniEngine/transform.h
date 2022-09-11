#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"
#include "rotation.h"

class Transform
{
public:
	Transform() {};

	Transform& identity();

	void setMatrix(const Matrix4 &m);
	Matrix4 getMatrix() const;

private:
	Vector3 m_translation;
	Rotation m_rotation;
	Vector3 m_scale;

	Rotation m_pivotOrientation;
	Vector3 m_pivotPosition;
};

#endif