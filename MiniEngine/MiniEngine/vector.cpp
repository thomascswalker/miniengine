#include "vector.h"

std::string Vector2::toString()
{
	return std::format("[{:.2f}, {:.2f}]", _x, _y);;
}

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
	_x = v.x();
	_y = v.y();
	_z = 0.0;
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
	_x = v.x();
	_y = v.y();
	_z = v.z();
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

double cross(const Vector2& v1, const Vector2& v2)
{
	return v1.x() * v2.y() - v2.x() * v1.y();
}

Vector3 cross(const Vector3& v1, const Vector3& v2)
{
	double x = (v1.y() * v2.z()) - (v1.z() * v2.y());
	double y = (v1.z() * v2.x()) - (v1.x() * v2.z());
	double z = (v1.x() * v2.y()) - (v1.y() * v2.x());
	return Vector3(x, y, z);
}
