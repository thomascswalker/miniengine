#ifndef ROTATION_H
#define ROTATION_H

#include "vector.h"

class Rotation
{
public:
	Rotation() { };
	Rotation(const Vector3& axis, const double angle)
	{
		setAxisAngle(axis, angle);
	};

	Rotation& setAxisAngle(const Vector3& axis, double angle)
	{
		m_axis = axis;
		m_angle = angle;
		m_axis.normalize();
		return *this;
	};

	static Rotation& identity()
	{
		Rotation rotation = Rotation(Vector3(0, 0, 0), 0);
		return rotation;
	};

private:
	Vector3 m_axis;
	double m_angle;
};

#endif