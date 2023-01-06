#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <string>
#include <cmath>

#include "api.h"

namespace Graphics {
using namespace Graphics;

class Vector2
{
public:
	double _x = 0;
	double _y = 0;

	// Constructors
	Vector2() : _x(0), _y(0) {};
	Vector2(double x, double y)
		: _x(x), _y(y) {};

	// Converts the X, Y values to a string
	[[nodiscard]] std::string toString() const;

	// Operators
	Vector2 operator + (Vector2& v) const;
	Vector2 operator - (Vector2& v) const;
	Vector2 operator * (Vector2& v) const;
	Vector2 operator / (Vector2& v) const;
	bool operator < (Vector2& v) const;
	bool operator > (Vector2& v) const;
	bool operator == (Vector2& v) const;
};

class Vector3
{
public:
	double _x = 0;
	double _y = 0;
	double _z = 0;

	// Constructors
	Vector3();
	Vector3(double x, double y, double z);
	explicit Vector3(double v);
	explicit Vector3(Vector2& v);
	Vector3(Vector2& v, double z);

	static Vector3 forward() { return {0.0, 0.0, 1.0}; }
	static Vector3 up() { return {0.0, 1.0, 0.0}; }
	static Vector3 right() { return {1.0, 0.0, 0.0}; }

	// Converts the X, Y, Z values to a string
    [[nodiscard]] std::string toString() const;

	// Normalizes the length of the vector so length == 1
	void normalize();

	template <typename T>
    void rescale(T amin, T amax, T bmin, T bmax)
    {
        _x = (((_x - amin) * (bmax - bmin)) / (amax - amin)) + bmin;
        _y = (((_y - amin) * (bmax - bmin)) / (amax - amin)) + bmin;
        _z = (((_z - amin) * (bmax - bmin)) / (amax - amin)) + bmin;
    }

	static Vector3 identity();
	[[nodiscard]] double length() const;
	void set(double x, double y, double z);

	// Operators
	double operator [] (int i) const;
	bool operator == (const Vector3& v) const;
	Vector3 operator + (Vector3& v) const;
	Vector3 operator + (const Vector3& v) const;
	Vector3 operator += (Vector3& v) const;
	Vector3 operator += (const Vector3& v) const;
	Vector3 operator - (Vector3& v) const;
	Vector3 operator - (const Vector3& v) const;
	Vector3 operator * (const Vector3& v) const;
	Vector3 operator * (Vector3& v) const;
	Vector3 operator * (double d) const;
	Vector3 operator / (Vector3& v) const;
	Vector3 operator / (double d) const;
	bool operator < (double d) const;
	bool operator < (Vector3& v) const;
	bool operator > (Vector3& v) const;
	bool operator == (Vector3& v) const;
	Vector3 operator - () const;
	Vector3 operator *= (Vector3& v) const;
	Vector3 operator *= (double d) const;
};

class Vector4
{
public:
	double _x = 0;
	double _y = 0;
	double _z = 0;
	double _w = 0;

	Vector4();
	explicit Vector4(double d);
	Vector4(double x, double y, double z, double w);
	Vector4(const Vector3& v, double w);

	// Converts the X, Y, Z, W values to a string
    // Converts the X, Y, Z values to a string
    [[nodiscard]] std::string toString() const;

	void normalize();
	static Vector4 identity();
	[[nodiscard]] double length() const { return sqrt(_x * _x + _y * _y + _z * _z + _w * _w); }
	void set(double x, double y, double z, double w) {_x = x, _y = y, _z = z, _w = w;}
	
	// Operators
	Vector4 operator + (Vector4& v) const;
	Vector4 operator - (Vector4& v) const;
	Vector4 operator * (Vector4& v) const;
	Vector4 operator / (Vector4& v) const;
	bool operator < (Vector4& v) const;
	bool operator > (Vector4& v) const;
	bool operator == (Vector4& v) const;
	Vector4 operator *= (Vector4& v) const;

};

}

#endif