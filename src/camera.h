#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"
#include "maths.h"


namespace Graphics {
using namespace Graphics;

class Camera : public Object
{
public:
	Camera();
	
	const Vector3& getTarget() { return m_target; }

	const double getFieldOfView() { return m_fieldOfView; }
	void setFieldOfView(double fov) { m_fieldOfView = clamp(fov, 1.0, 179.0); }
	const double getNearClip() { return m_nearClip; }
	const double getFarClip() { return m_farClip; }

	const Matrix4 getViewMatrix();
	const Matrix4 getNewViewMatrix() { return m_viewMatrix; }
	inline void	updateViewMatrix()
	{
		Matrix4 m = lookAt(getTranslation(), m_target, Vector3::up());
		m_transform.setMatrix(m);
	}
	inline void updateViewMatrix(Matrix4& m)
	{
		m_viewMatrix = m;
	}

	const Matrix4			getProjectionMatrix(const double width, const double height);

private:
	Vector3 m_target		= Vector3(0.0);

	double m_fieldOfView	= 45.0;	// Degrees
	double m_nearClip		= 0.0;
	double m_farClip		= 1000.0;

	Matrix4 m_viewMatrix = Matrix4();
};

}

#endif