#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <string>
#include <format>
#include <cmath>

#include "api.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

class Vector2
{
public:
	double _x = 0;
	double _y = 0;

	// Constructors
	Vector2() : _x(0), _y(0) {};
	Vector2(double x, double y) : _x(x), _y(y) {};

	// Methods
	double x() const { return _x; }
	void setX(double x) { _x = x; }
	double y() const { return _y; }
	void setY(double y) { _y = y; }

	Vector2 copy() { return Vector2(_x, _y); }

	// Converts the X, Y values to a string
	std::string toString();

	// Operators
	Vector2 operator + (Vector2& v) const
	{
		return Vector2(_x + v._x, _y + v._y);
	}
	
	Vector2 operator - (Vector2& v) const
	{
		return Vector2(_x - v._x, _y - v._y);
	}
	
	Vector2 operator * (Vector2& v) const
	{
		return Vector2(_x * v._x, _y * v._y);
	}
	
	Vector2 operator / (Vector2& v) const
	{
		return Vector2(_x / v._x, _y / v._y);
	}

	bool operator < (Vector2& v) const
	{
		return _x < v._x && _y < v._y;
	}

	bool operator > (Vector2& v) const
	{
		return _x > v._x && _y > v._y;
	}

	bool operator == (Vector2& v) const
	{
		return _x - v._x < DBL_EPSILON && _y - v._y < DBL_EPSILON;
	}
};

class Vector3 : public Vector2
{
public:
	double _z = 0;

	// Constructors
	Vector3();
	Vector3(double x, double y, double z);
	Vector3(double v) { *this = Vector3(v, v, v); }
	Vector3(Vector2& v);
	Vector3(Vector2& v, double z);

	Vector3 copy() { return Vector3(_x, _y, _z); }

	static Vector3 forward() { return Vector3(0.0, 0.0, 1.0); }
	static Vector3 up() { return Vector3(0.0, 1.0, 0.0); }
	static Vector3 right() { return Vector3(1.0, 0.0, 0.0); }

	// Operators
	double operator [] (int i) const
	{
		if (i == 0)
		{
			return _x;
		}
		else if (i == 1)
		{
			return _y;
		}
		else if (i == 2)
		{
			return _z;
		}

		return 0;
	}

	bool operator == (const Vector3& v) const
	{
		return (_x == v._x &&
				_y == v._y &&
				_z == v._z);
	}

	Vector3 operator + (Vector3& v) const
	{
		return Vector3(_x + v._x, _y + v._y, _z + v._z);
	}

	Vector3 operator + (const Vector3& v) const
	{
		return Vector3(_x + v._x, _y + v._y, _z + v._z);
	}

	Vector3 operator += (Vector3& v) const
	{
		return Vector3(_x + v._x, _y + v._y, _z + v._z);
	}

	Vector3 operator += (const Vector3& v) const
	{
		return Vector3(_x + v._x, _y + v._y, _z + v._z);
	}
	
	Vector3 operator - (Vector3& v) const
	{
		return Vector3(_x - v._x, _y - v._y, _z - v._z);
	}

	const Vector3 operator - (const Vector3& v) const
	{
		return Vector3(_x - v._x, _y - v._y, _z - v._z);
	}
	
	const Vector3 operator * (const Vector3& v) const
	{
		return Vector3(_x * v._x, _y * v._y, _z * v._z);
	}

	Vector3 operator * (Vector3& v) const
	{
		return Vector3(_x * v._x, _y * v._y, _z * v._z);
	}

	Vector3 operator * (double d) const
	{
		return Vector3(_x * d, _y * d, _z * d);
	}
	
	Vector3 operator / (Vector3& v) const
	{
		return Vector3(_x / v._x, _y / v._y, _z / v._z);
	}

	Vector3 operator / (double d) const
	{
		return Vector3(_x / d, _y / d, _z / d);
	}

	bool operator < (double d)
	{
		return _x < d && _y < d && _z < d;
	}

	bool operator < (Vector3& v)
	{
		return _x < v._x && _y < v._y && _z < v._z;
	}

	bool operator < (Vector3& v) const
	{
		return _x < v._x && _y < v._y && _z < v._z;
	}

	bool operator > (Vector3& v) const
	{
		return _x > v._x && _y > v._y && _z > v._z;
	}

	bool operator == (Vector3& v) const
	{
		return _x - v._x < 0.0001f && _y - v._y < 0.0001f && _z - v._z < 0.0001f;
	}

	Vector3 operator -() const
	{
		return Vector3(-_x, -_y, -_z);
	}

	Vector3 operator *= (Vector3& v) const
	{	
		return (*this * v);
	}

	Vector3 operator *= (double d) const
	{	
		return (*this * d);
	}

	// Methods
	double z() const { return _z; }
	void setZ(double z) { _z = z; }

	// Converts the X, Y, Z values to a string
	std::string toString();

	// Normalizes the length of the vector so length == 1
	void normalize();
	static Vector3 identity();
	double length()
	{
		return sqrt(_x * _x + _y * _y + _z * _z);
	}
	void set(double x, double y, double z) {_x = x, _y = y, _z = z;}
};

class Vector4 : public Vector3
{
public:
	double _w = 0;

	Vector4();
	Vector4(double d);
	Vector4(double x, double y, double z, double w);
	Vector4(const Vector3& v, double w);

	Vector4 copy() { return Vector4(_x, _y, _z, _w); }

	// Operators
	Vector4 operator + (Vector4& v) const
	{
		return Vector4(_x + v._x, _y + v._y, _z + v._z, _w + v._w);
	}
	
	Vector4 operator - (Vector4& v) const
	{
		return Vector4(_x - v._x, _y - v._y, _z - v._z, _w - v._w);
	}
	
	Vector4 operator * (Vector4& v) const
	{
		return Vector4(_x * v._x, _y * v._y, _z * v._z, _w * v._w);
	}
	
	Vector4 operator / (Vector4& v) const
	{
		return Vector4(_x / v._x, _y / v._y, _z / v._z, _w / v._w);
	}

	bool operator < (Vector4& v) const
	{
		return _x < v._x && _y < v._y && _z < v._z && _w < v._w;
	}

	bool operator > (Vector4& v) const
	{
		return _x > v._x && _y > v._y && _z > v._z && _w > v._w;
	}

	bool operator == (Vector4& v) const
	{
		return _x - v._x < DBL_EPSILON && _y - v._y < DBL_EPSILON && _z - v._z < DBL_EPSILON && _w - v._w < DBL_EPSILON;
	}

	Vector4 operator *= (Vector4& v)
	{	
		return (*this * v);
	}

	// Methods
	double w() const { return _w; }
	void setW(double w) { _w = w; }

	// Converts the X, Y, Z, W values to a string
	std::string toString();

	void normalize();
	static Vector4 identity();
	double length() { return sqrt(_x * _x + _y * _y + _z * _z + _w * _w); }
	void set(double x, double y, double z, double w) {_x = x, _y = y, _z = z, _w = w;}
};

MINI_NAMESPACE_CLOSE

#endif