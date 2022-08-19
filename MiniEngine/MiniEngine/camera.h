#ifndef CAMERA_H
#define CAMERA_H
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include "vector.h"

constexpr double x = 43.2667;

class Camera
{
public:
	Camera() {};

	float getFov() { m_fov; }

private:
	float m_fov = 90.0f;
	float m_pitch = 0;
	float m_yaw = 0;
	float m_roll = 0;

	float m_near = 0;
	float m_far = 100;
};
