#ifndef CAMERA_H
#define CAMERA_H

#include "math.h"
#include "matrix.h"
#include "vector.h"

class Camera
{
public:
	Camera();

	const float getFieldOfView() { return m_fieldOfView; }
	const float getNearClip() { return m_nearClip; }
	const float getFarClip() { return m_farClip; }

	void move(float x, float y, float z);
	void move(Vector3 &vec);
	
	const Vector3 getPosition() { return m_position; }
	void setPosition(float x, float y, float z);
	void setPosition(Vector3 &vec);

	Matrices::Matrix4 getCameraMatrix();

	Matrices::Matrix4 getProjectionMatrix() { return m_projectionMatrix; }
	void setProjectionMatrix(const float &angle, const float &nearClip, const float &farClip);

private:
	Vector3 m_position;
	float m_fieldOfView = 90.0f;
	float m_nearClip = 0.1f;
	float m_farClip = 100.0f;
	Matrices::Matrix4 m_projectionMatrix;
};

#endif