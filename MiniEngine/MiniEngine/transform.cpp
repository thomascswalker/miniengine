#include "transform.h"

Transform& Transform::identity()
{
    m_translation.set(0, 0, 0);
    m_rotation.identity();
    m_scale.set(1, 1, 1);

    m_pivotOrientation = Rotation::identity();
    m_pivotPosition = Vector3::identity();

    return *this;
}

// http://www.codinglabs.net/article_world_view_projection_matrix.aspx
Matrix4 Transform::getMatrix() const
{
    // Translate
    Matrix4 t;
    t.set(1.0, 0.0, 0.0, m_translation.x(),
          0.0, 1.0, 0.0, m_translation.y(),
          0.0, 0.0, 1.0, m_translation.z(),
          0.0, 0.0, 0.0, 1.0);

    // Rotate
    Quaternion q = m_rotation.getQuaternion();
    Vector3 r = q.getImaginary();

    Matrix4 rx;
    rx.set(1.0, 0.0,         0.0,           0.0,
           0.0, cosf(r.x()), -sinf(r.x()),  0.0,
           0.0, sinf(r.x()), cosf(r.x()),   0.0,
           0.0, 0.0,         0.0,           1.0);

    Matrix4 ry;
    ry.set(cosf(r.y()),  0.0, sinf(r.y()),  0.0,
           0.0,          1.0, 0.0,          0.0,
           -sinf(r.y()), 0.0, cosf(r.y()),  0.0,
           0.0,          0.0, 0.0,          1.0);

    Matrix4 rz;
    rz.set(cosf(r.y()), -sinf(r.y()), 0.0, 0.0,
           sinf(r.y()), cosf(r.y()),  0.0, 0.0,
           0.0,         0.0,          1.0, 0.0,
           0.0,         0.0,          0.0, 1.0);

    // Scale
    Matrix4 s;
    s.set(m_scale.x(), 0.0,         0.0,         0.0,
          0.0,         m_scale.y(), 0.0,         0.0,
          0.0,         0.0,         m_scale.z(), 0.0,
          0.0,         0.0,         0.0,         1.0);

    return rx * ry * rz * t * s;
}

void Transform::setTranslation(const Vector3& t)
{
    m_translation = t;
}

void Transform::setRotation(const Rotation& r)
{
    m_rotation = r;
}

void Transform::setScale(const Vector3& s)
{
    m_scale = s;
}

const Vector3& Transform::getForward()
{
    Matrix4 m = getMatrix();
    return Vector3(m[0][0], m[1][0], m[2][0]);
}

const Vector3& Transform::getRight()
{
    Matrix4 m = getMatrix();
    return Vector3(m[0][1], m[1][1], m[2][1]);
}

const Vector3& Transform::getUp()
{
    Matrix4 m = getMatrix();
    return Vector3(m[0][2], m[1][2], m[2][2]);
}