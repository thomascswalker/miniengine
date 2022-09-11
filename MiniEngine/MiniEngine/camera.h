#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"
#include "math.h"

class Camera : public Object
{
public:
	Camera();

	const float getFieldOfView() { return m_fieldOfView; }
	const float getNearClip() { return m_nearClip; }
	const float getFarClip() { return m_farClip; }

	//void move(float x, float y, float z);
	//void move(Vector3 &vec);
	//
	//const Vector3 getPosition() { return m_position; }
	//void setPosition(float x, float y, float z);
	//void setPosition(Vector3 &vec);

	Matrix4 getProjectionMatrix();

private:
	//Vector3 m_position;
	float m_fieldOfView = 120.0f;
	float m_nearClip	= 0.1f;
	float m_farClip		= 100.0f;
};

#endif