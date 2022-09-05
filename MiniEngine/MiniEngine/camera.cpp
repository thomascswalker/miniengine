#include "camera.h"

Camera::Camera()
    : m_position(Vector3()),
      m_projectionMatrix(Matrices::Matrix4())
{
    setProjectionMatrix(m_fieldOfView, m_nearClip, m_farClip);
}

void Camera::move(float x, float y, float z)
{
    m_position.setX(m_position.x() + x);
    m_position.setY(m_position.y() + y);
    m_position.setZ(m_position.z() + z);
}

void Camera::move(Vector3& vec)
{
    move(vec.x(), vec.y(), vec.z());
}

void Camera::setPosition(float x, float y, float z)
{
    m_position.setX(x);
    m_position.setY(y);
    m_position.setZ(z);
}

void Camera::setPosition(Vector3& vec)
{
    setPosition(vec.x(), vec.y(), vec.z());
}

Matrices::Matrix4 Camera::getCameraMatrix()
{
    auto m = Matrices::Matrix4();
    m.setIndex(9, m_position.x());
    m.setIndex(10, m_position.y());
    m.setIndex(11, m_position.z());
    return m;
}

/*
https://www.scratchapixel.com/code.php?id=4&origin=/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix
*/
void Camera::setProjectionMatrix(const float &angle, const float &nearClip, const float &farClip)
{
    float scale = 1.0f / tan(angle * 0.5 * Math::PI / 180.0f);

    // Reset projection matrix
    m_projectionMatrix = Matrices::Matrix4();

    // Distance between near clip and far clip
    float deltaClip = farClip - nearClip;

    m_projectionMatrix.setIndex(0, scale);                              // Scale the X coordinate of the projected point
    m_projectionMatrix.setIndex(5, scale);                              // Scale the Y coordinate of the projected point
    m_projectionMatrix.setIndex(10, -farClip / deltaClip);              // Remap Z to [0, 1]
    m_projectionMatrix.setIndex(11, -farClip * nearClip / deltaClip);   // Remap Z to [0, 1]
    m_projectionMatrix.setIndex(14, -1);                                // Set W to -Z
    m_projectionMatrix.setIndex(15, 0);
}
