#include "vector.h"

std::string Vector2::toString()
{
	return std::format("[{}, {}]", _x, _y);;
}

Vector3::Vector3()
{
	_x = 0;
	_y = 0;
	_z = 0;
};
Vector3::Vector3(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;
};

std::string Vector3::toString()
{
	return std::format("[{}, {}, {}]", _x, _y, _z);
}


void Vector3::normalize()
{
	float l = length();
	_x /= l;
	_y /= l;
	_z /= l;
}

Vector3 Vector3::identity()
{
	return Vector3(1.0f, 1.0f, 1.0f);
}


Vector4::Vector4()
{
	_x = 0;
	_y = 0;
	_z = 0;
	_z = 0;
}

Vector4::Vector4(float x, float y, float z, float w)
{
	_x = x;
	_y = y;
	_z = z;
	_z = w;
}

std::string Vector4::toString()
{
	return std::format("[{}, {}, {}, {}]", _x, _y, _z, _w);
}

void Vector4::normalize()
{
	float l = length();
	_x /= l;
	_y /= l;
	_z /= l;
	_w /= l;
}

Vector4 Vector4::identity()
{
	return Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

float cross(const Vector2& v1, const Vector2& v2)
{
	return v1.x() * v2.y() - v2.x() * v1.y();
}

Vector3 cross(const Vector3& v1, const Vector3& v2)
{
	float x = (v1.y() * v2.z()) - (v1.z() * v2.y());
	float y = (v1.z() * v2.x()) - (v1.x() * v2.z());
	float z = (v1.x() * v2.y()) - (v1.y() * v2.x());
	return Vector3(x, y, z);
}
