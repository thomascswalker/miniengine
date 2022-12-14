#ifndef ROTATION_H
#define ROTATION_H

#include "quaternion.h"
#include "vector.h"

namespace Graphics {
using namespace Graphics;

class Rotation
{
public:
	Rotation() : m_axis(Vector3()), m_angle(90.0) {};
	Rotation(const Vector3& axis, const double angle)
	{
		setAxisAngle(axis, angle);
	};
	Rotation(Quaternion& q)
	{
		setQuaternion(q);
	}

	Rotation& setIdentity()
	{
		m_axis.set(1.0, 0.0, 0.0);
		m_angle = 0.0;
		return *this;
	}

	Rotation& setQuaternion(Quaternion& q)
	{
		double len = q.getImaginary().length();

		if (len < EPSILON) // Minimum vector length
		{
			auto r = q.getReal();
			double x = acos(clamp(r, -1.0, 1.0));
			setAxisAngle(q.getImaginary() / len, 2.0 * RADIANS(x));
		}
		else
		{
			setIdentity();
		}

		return *this;
	}

	Rotation& setAxisAngle(const Vector3& axis, double angle)
	{
		m_axis = axis;
		m_angle = angle;
		m_axis.normalize();
		return *this;
	};

	static Rotation identity()
	{
		Rotation rotation = Rotation(Vector3(0, 0, 0), 0);
		return rotation;
	};

	static Rotation fromEulerAngles(double yaw, double pitch, double roll)
	{
		double qx = sin(roll/2.0) * cos(pitch/2.0) * cos(yaw/2.0) - cos(roll/2.0) * sin(pitch/2.0) * sin(yaw/2.0);
		double qy = cos(roll/2.0) * sin(pitch/2.0) * cos(yaw/2.0) + sin(roll/2.0) * cos(pitch/2.0) * sin(yaw/2.0);
		double qz = cos(roll/2.0) * cos(pitch/2.0) * sin(yaw/2.0) - sin(roll/2.0) * sin(pitch/2.0) * cos(yaw/2.0);
		double qw = cos(roll/2.0) * cos(pitch/2.0) * cos(yaw/2.0) + sin(roll/2.0) * sin(pitch/2.0) * sin(yaw/2.0);

		Quaternion q(qw, Vector3(qx, qy, qz));
		return Rotation(q);
	}

	const Vector3& getAxis()
	{
		return m_axis;
	}

	double getAngle()
	{
		return m_angle;
	}

	Rotation getInverse() const
	{
		return Rotation(m_axis, -m_angle);
	}
	Quaternion getQuaternion() const
	{
		double radians = RADIANS(m_angle) / 2.0;

		double sinR = sin(radians);
		double cosR = cos(radians);

		Vector3 axis = m_axis * sinR;
		auto q = Quaternion(cosR, axis);
		return q.getNormalized();
	}

	std::string toString()
	{
		return "Axis: " + m_axis.toString() + ", Angle: " + std::to_string(m_angle);
	}

	Rotation& operator += (const Rotation& r)
	{
		m_axis += r.m_axis;
		m_angle += r.m_angle;
		return *this;
	}

	const Rotation& operator += (const Rotation& r) const
	{
		Rotation tmp = *this;
		tmp.m_axis += r.m_axis;
		tmp.m_angle += r.m_angle;
		return *this;
	}

	friend inline Rotation operator +(const Rotation& r1, const Rotation& r2)
	{
		return r1 += r2;
	}

private:
	Vector3 m_axis;
	double m_angle;
};

}

#endif