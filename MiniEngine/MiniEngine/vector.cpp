#include "vector.h"

std::string Vector2::toString()
{
	return std::format("[{}, {}]", _x, _y);;
}

double Vector2::cross(Vector2& other)
{
	return (_x * other.y()) - (other.x() * _y);
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
	_x = _x / l;
	_y = _y / l;
	_z = _z / l;
}

Vector3 Vector3::identity()
{
	return Vector3(1.0f, 1.0f, 1.0f);
}

float Vector3::dot(Vector3& other)
{
	return _x * other.x() + _y * other.y() + _z * other.z();
}

Vector3 Vector3::cross(Vector3& other)
{
	return Vector3(_y * other.z() - _z * other.y(),
				   _z * other.x() - _x * other.z(),
				   _x * other.y() - _y * other.x());
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
	_x = _x / l;
	_y = _y / l;
	_z = _z / l;
	_w = _w / l;
}

Vector4 Vector4::identity()
{
	return Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}
