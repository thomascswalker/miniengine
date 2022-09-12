#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"
#include "math.h"

class Camera : public Object
{
public:
	Camera();

	const float		getFieldOfView() { return m_fieldOfView; }
	const float		getNearClip() { return m_nearClip; }
	const float		getFarClip() { return m_farClip; }

	const Matrix4   getViewMatrix();
	const Matrix4	getProjectionMatrix(const float width, const float height);

private:
	float m_fieldOfView = 43.0;
	float m_nearClip	= 1;
	float m_farClip		= 1000;
};

#endif