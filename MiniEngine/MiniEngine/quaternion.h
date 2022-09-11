#ifndef QUATERNION_H
#define QUATERNION_H

#include "math.h"

class Quaternion
{
public:
	// Constructors
	Quaternion()
		: m_real(0), m_imaginary(Vector3()) {};
	Quaternion(double real)
		: m_real(real), m_imaginary(Vector3()) {};
	Quaternion(double real, const Vector3& imaginary)
		: m_real(real), m_imaginary(imaginary) {};

	// Methods
	static Quaternion	getIdentity()
	{
		return Quaternion(1.0, Vector3(0, 0, 0));
	}
	Quaternion			getNormalized() const;

	double				getReal() { return m_real; }
	Vector3				getImaginary() { return m_imaginary; }

private:
	// Properties
	double m_real;
	Vector3 m_imaginary;

	// Private methods
	double p_getLengthSquared() const
	{
		return (m_real * m_real * Math::dot(m_imaginary, m_imaginary));
	}
};

#endif

/*
	p = vec3(1.00, 0.25, 1.20)	// point we want to rotate
	a = vec3(45, 90, 30);		// rotation angle
	a.normalize()				// so a.x + a.y + a.z = 1

	q = (cos(40 / 2) + sin (40 / 2)) * (a.x - a.y - a.z)

	rp = q * p * q^-1
*/