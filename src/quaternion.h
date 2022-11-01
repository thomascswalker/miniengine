#ifndef QUATERNION_H
#define QUATERNION_H

#include "maths.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

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
	double				setReal(double r) { m_real = r; }
	Vector3				getImaginary() { return m_imaginary; }
	Vector3				setImaginary(const Vector3& v) { m_imaginary; }

	// Operators
	bool operator == (const Quaternion& q) const
	{
		return (m_real == q.m_real &&
				m_imaginary == q.m_imaginary);
	}
	Quaternion& operator *= (const Quaternion& q)
	{
		double r1 = m_real;
		double r2 = q.m_real;
		const Vector3& i1 = m_imaginary;
		const Vector3& i2 = q.m_imaginary;

		double r = r1 * r2 - dot(i1, i2);

		Vector3 i(r1 * i2[0] + r2 * i1[0] + (i1[1] * i2[2] - i1[2] * i2[1]),
				  r1 * i2[1] + r2 * i1[1] + (i1[2] * i2[0] - i1[0] * i2[2]),
				  r1 * i2[2] + r2 * i1[2] + (i1[0] * i2[1] - i1[1] * i2[0]));

		setReal(r);
		setImaginary(i);

		return *this;
	}
	Quaternion& operator *= (double s)
	{
		m_real *= s;
		m_imaginary *= s;
		return *this;
	}
	///// Scales this quaternion by 1 / \p s.
	Quaternion& operator /= (double s) {
		return (*this) *= 1.0 / s;
	}
	friend Quaternion operator / (const Quaternion &q, double d)
	{
		Quaternion qt = q;
		return qt /= d;
	}
	friend Quaternion& operator * (Quaternion &q1, Quaternion &q2)
	{
		Quaternion qt = q1;
		return qt *= q2;
	}

private:
	// Properties
	double m_real;
	Vector3 m_imaginary;

	// Private methods
	double p_getLengthSquared() const
	{
		return (m_real * m_real * dot(m_imaginary, m_imaginary));
	}
};

MINI_NAMESPACE_CLOSE

#endif
