#include "vector.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

Vector3::Vector3()
{
	_x = 0;
	_y = 0;
	_z = 0;
};

Vector3::Vector3(double x, double y, double z)
{
	_x = x;
	_y = y;
	_z = z;
};

Vector3::Vector3(Vector2& v)
{
	_x = v._x;
	_y = v._y;
	_z = 0.0;
};

Vector3::Vector3(Vector2& v, double z)
{
	_x = v._x;
	_y = v._y;
	_z = z;
};

std::string Vector3::toString()
{
	return std::format("[{:.2f}, {:.2f}, {:.2f}]", _x, _y, _z);
}


void Vector3::normalize()
{
	double l = length();
	_x /= l;
	_y /= l;
	_z /= l;
}

Vector3 Vector3::identity()
{
	return Vector3(1.0, 1.0, 1.0);
}


Vector4::Vector4()
{
	_x = 0;
	_y = 0;
	_z = 0;
	_w = 0;
}

Vector4::Vector4(double d)
{
	_x = d;
	_y = d;
	_z = d;
	_w = d;
}

Vector4::Vector4(double x, double y, double z, double w)
{
	_x = x;
	_y = y;
	_z = z;
	_w = w;
}

Vector4::Vector4(const Vector3& v, double w)
{
	_x = v._x;
	_y = v._y;
	_z = v._z;
	_w = w;
}

std::string Vector4::toString()
{
	return std::format("[{:.2f}, {:.2f}, {:.2f}, {:.2f}]", _x, _y, _z, _w);
}

void Vector4::normalize()
{
	double l = length();
	_x /= l;
	_y /= l;
	_z /= l;
	_w /= l;
}

Vector4 Vector4::identity()
{
	return Vector4(1.0, 1.0, 1.0, 1.0);
}

MINI_NAMESPACE_CLOSE