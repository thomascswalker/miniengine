#ifndef VECTOR_H
#define VECTOR_H
#endif

#include <iostream>
#include <string>
#include <format>

class Vector3
{
private:
	float _x = 0;
	float _y = 0;
	float _z = 0;

public:

	// Constructors
	Vector3(float x, float y, float z) : _x(x), _y(y), _z(z) {};

	// Operators
	Vector3 operator + (const Vector3& v) const
	{
		return Vector3(_x + v.x(), _y + v.y(), _z + v.z());
	}
	
	Vector3 operator - (const Vector3& v) const
	{
		return Vector3(_x - v.x(), _y - v.y(), _z - v.z());
	}
	
	Vector3 operator * (const Vector3& v) const
	{
		return Vector3(_x * v.x(), _y * v.y(), _z * v.z());
	}
	
	Vector3 operator / (const Vector3& v) const
	{
		return Vector3(_x / v.x(), _y / v.y(), _z / v.z());
	}

	// Methods
	float x() const { return _x; }
	float y() const { return _y; }
	float z() const { return _z; }

	// Converts the X, Y, Z values to a string
	std::string toString();

	// Normalizes the length of the vector so length == 1
	void normalize();
	static Vector3 identity();
};

