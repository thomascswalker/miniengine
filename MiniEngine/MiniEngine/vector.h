#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <string>
#include <format>

class Vector2
{
protected:
	float _x = 0;
	float _y = 0;

public:
	// Constructors
	Vector2() : _x(0), _y(0) {};
	Vector2(float x, float y) : _x(x), _y(y) {};

	// Methods
	float x() const { return _x; }
	void setX(float x) { _x = x; }
	float y() const { return _y; }
	void setY(float y) { _y = y; }

	// Converts the X, Y values to a string
	std::string toString();

	// Operators
	Vector2 operator + (const Vector2& v) const
	{
		return Vector2(_x + v.x(), _y + v.y());
	}
	
	Vector2 operator - (const Vector2& v) const
	{
		return Vector2(_x - v.x(), _y - v.y());
	}
	
	Vector2 operator * (const Vector2& v) const
	{
		return Vector2(_x * v.x(), _y * v.y());
	}
	
	Vector2 operator / (const Vector2& v) const
	{
		return Vector2(_x / v.x(), _y / v.y());
	}
};

class Vector3 : public Vector2
{
protected:
	float _z = 0;

public:

	// Constructors
	Vector3();
	Vector3(float x, float y, float z);

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
	float z() const { return _z; }
	void setZ(float z) { _z = z; }

	// Converts the X, Y, Z values to a string
	std::string toString();

	// Normalizes the length of the vector so length == 1
	void normalize();
	static Vector3 identity();
	float dot(const Vector3& other);
	Vector3 cross(const Vector3& other);
	float length() { return sqrtf(_x * _x + _y * _y + _z * _z); }
	void set(float x, float y, float z) {_x = x, _y = y, _z = z;}
};

class Vector4 : public Vector3
{
protected:
	float _w = 0;

public:
	Vector4();
	Vector4(float x, float y, float z, float w);

	// Operators
	Vector4 operator + (const Vector4& v) const
	{
		return Vector4(_x + v.x(), _y + v.y(), _z + v.z(), _w + v.w());
	}
	
	Vector4 operator - (const Vector4& v) const
	{
		return Vector4(_x - v.x(), _y - v.y(), _z - v.z(), _w - v.w());
	}
	
	Vector4 operator * (const Vector4& v) const
	{
		return Vector4(_x * v.x(), _y * v.y(), _z * v.z(), _w * v.w());
	}
	
	Vector4 operator / (const Vector4& v) const
	{
		return Vector4(_x / v.x(), _y / v.y(), _z / v.z(), _w / v.w());
	}

	// Methods
	float w() const { return _w; }
	void setW(float w) { _w = w; }

	// Converts the X, Y, Z, W values to a string
	std::string toString();

	void normalize();
	static Vector4 identity();
	float length() { return sqrtf(_x * _x + _y * _y + _z * _z + _w * _w); }
	void set(float x, float y, float z, float w) {_x = x, _y = y, _z = z, _w = w;}
};

#endif