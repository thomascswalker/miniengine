#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"
#include "maths.h"


MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

class Camera : public Object
{
public:
	Camera();

	const double			getFieldOfView() { return m_fieldOfView; }
	void					setFieldOfView(double fov) { m_fieldOfView = clamp(fov, 1.0, 179.0); }
	const double			getNearClip() { return m_nearClip; }
	const double			getFarClip() { return m_farClip; }

	const Matrix4			getViewMatrix();
	const Matrix4			getProjectionMatrix(const double width, const double height);

private:
	double m_fieldOfView	= 43.0;	// Degrees
	double m_nearClip		= 0.0;
	double m_farClip		= 1000.0;
};

MINI_NAMESPACE_CLOSE

#endif