#include "camera.h"

Camera::Camera()
{
    
}
//
//void Camera::move(float x, float y, float z)
//{
//    m_position.setX(m_position.x() + x);
//    m_position.setY(m_position.y() + y);
//    m_position.setZ(m_position.z() + z);
//}
//
//void Camera::move(Vector3& vec)
//{
//    move(vec.x(), vec.y(), vec.z());
//}
//
//void Camera::setPosition(float x, float y, float z)
//{
//    m_position.setX(x);
//    m_position.setY(y);
//    m_position.setZ(z);
//}
//
//void Camera::setPosition(Vector3& vec)
//{
//    setPosition(vec.x(), vec.y(), vec.z());
//}
//
//Matrix4 Camera::getCameraMatrix()
//{
//    //auto m = Matrix4();
//    //m[], m_position.x());
//    //m.setIndex(10, m_position.y());
//    //m.setIndex(11, m_position.z());
//    //return m;
//}

/*
https://www.scratchapixel.com/code.php?id=4&origin=/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix
*/
Matrix4 Camera::getProjectionMatrix()
{
    float scale = 1.0f / tan(m_fieldOfView * 0.5 * Math::PI / 180.0f);

    // Reset projection matrix
    auto projectionMatrix = Matrix4();

    // Distance between near clip and far clip
    float deltaClip = m_farClip - m_nearClip;

    projectionMatrix[0][0] = scale;
    projectionMatrix[1][1] = scale;
    projectionMatrix[2][2] = -m_farClip / deltaClip;
    projectionMatrix[2][3] = -m_farClip * m_nearClip / deltaClip;
    projectionMatrix[3][2] = -1;
    projectionMatrix[3][3] = 0;

    return projectionMatrix;
}
