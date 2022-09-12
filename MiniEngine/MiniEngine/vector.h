#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <string>
#include <format>

class Vector2
{
protected:
	double _x = 0;
	double _y = 0;

public:
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
		return Vector2(_x + v.x(), _y + v.y());
	}
	
	Vector2 operator - (Vector2& v) const
	{
		return Vector2(_x - v.x(), _y - v.y());
	}
	
	Vector2 operator * (Vector2& v) const
	{
		return Vector2(_x * v.x(), _y * v.y());
	}
	
	Vector2 operator / (Vector2& v) const
	{
		return Vector2(_x / v.x(), _y / v.y());
	}

	bool operator < (Vector2& v) const
	{
		return _x < v.x() && _y < v.y();
	}

	bool operator > (Vector2& v) const
	{
		return _x > v.x() && _y > v.y();
	}

	bool operator == (Vector2& v) const
	{
		return _x - v.x() < 0.0001f && _y - v.y() < 0.0001f;
	}
};

class Vector3 : public Vector2
{
protected:
	double _z = 0;

public:

	// Constructors
	Vector3();
	Vector3(double x, double y, double z);

	Vector3 copy() { return Vector3(_x, _y, _z); }

	// Operators
	double operator [] (int i) const
	{
		switch (i)
		{
			case 0: return _x;
			case 1: return _y;
			case 2: return _z;
		}
	}

	bool operator == (const Vector3& v) const
	{
		return (_x == v.x() &&
				_y == v.y() &&
				_z == v.z());
	}

	Vector3 operator + (Vector3& v) const
	{
		return Vector3(_x + v.x(), _y + v.y(), _z + v.z());
	}

	Vector3 operator + (const Vector3& v) const
	{
		return Vector3(_x + v.x(), _y + v.y(), _z + v.z());
	}

	Vector3 operator += (Vector3& v) const
	{
		return Vector3(_x + v.x(), _y + v.y(), _z + v.z());
	}
	
	Vector3 operator - (Vector3& v) const
	{
		return Vector3(_x - v.x(), _y - v.y(), _z - v.z());
	}
	
	const Vector3 operator * (const Vector3& v) const
	{
		return Vector3(_x * v.x(), _y * v.y(), _z * v.z());
	}

	Vector3 operator * (Vector3& v) const
	{
		return Vector3(_x * v.x(), _y * v.y(), _z * v.z());
	}

	Vector3 operator * (double d) const
	{
		return Vector3(_x * d, _y * d, _z * d);
	}
	
	Vector3 operator / (Vector3& v) const
	{
		return Vector3(_x / v.x(), _y / v.y(), _z / v.z());
	}

	bool operator < (Vector3& v) const
	{
		return _x < v.x() && _y < v.y() && _z < v.z();
	}

	bool operator > (Vector3& v) const
	{
		return _x > v.x() && _y > v.y() && _z > v.z();
	}

	bool operator == (Vector3& v) const
	{
		return _x - v.x() < 0.0001f && _y - v.y() < 0.0001f && _z - v.z() < 0.0001f;
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
		return sqrtf((float)(_x * _x) + (float)(_y * _y) + (float)(_z * _z));
	}
	void set(double x, double y, double z) {_x = x, _y = y, _z = z;}
};

class Vector4 : public Vector3
{
protected:
	double _w = 0;

public:
	Vector4();
	Vector4(double x, double y, double z, double w);

	Vector4 copy() { return Vector4(_x, _y, _z, _w); }

	// Operators
	Vector4 operator + (Vector4& v) const
	{
		return Vector4(_x + v.x(), _y + v.y(), _z + v.z(), _w + v.w());
	}
	
	Vector4 operator - (Vector4& v) const
	{
		return Vector4(_x - v.x(), _y - v.y(), _z - v.z(), _w - v.w());
	}
	
	Vector4 operator * (Vector4& v) const
	{
		return Vector4(_x * v.x(), _y * v.y(), _z * v.z(), _w * v.w());
	}
	
	Vector4 operator / (Vector4& v) const
	{
		return Vector4(_x / v.x(), _y / v.y(), _z / v.z(), _w / v.w());
	}

	bool operator < (Vector4& v) const
	{
		return _x < v.x() && _y < v.y() && _z < v.z() && _w < v.w();
	}

	bool operator > (Vector4& v) const
	{
		return _x > v.x() && _y > v.y() && _z > v.z() && _w > v.w();
	}

	bool operator == (Vector4& v) const
	{
		return _x - v.x() < 0.0001f && _y - v.y() < 0.0001f && _z - v.z() < 0.0001f && _w - v.w() < 0.0001f;
	}

	// Methods
	double w() const { return _w; }
	void setW(double w) { _w = w; }

	// Converts the X, Y, Z, W values to a string
	std::string toString();

	void normalize();
	static Vector4 identity();
	double length() { return sqrtf((float)(_x * _x + _y * _y + _z * _z + _w * _w)); }
	void set(double x, double y, double z, double w) {_x = x, _y = y, _z = z, _w = w;}
};

double cross(const Vector2& v1, const Vector2& v2);
Vector3 cross(const Vector3& v1, const Vector3& v2);

#endif