#include <iostream>
#include "vector.h"


std::string Vector3::toString()
{
	return std::format("[{}, {}, {}]", _x, _y, _z);
}


void Vector3::normalize()
{
	_x = 1.0f / _x;
	_y = 1.0f / _y;
	_z = 1.0f / _z;
}

Vector3 Vector3::identity()
{
	return Vector3(1.0f, 1.0f, 1.0f);
}
