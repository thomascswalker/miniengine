#include "transform.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

Transform& Transform::identity()
{
    m_translation.set(0.0, 0.0, 0.0);
    m_rotation = Rotation::identity();
    m_scale.set(1.0, 1.0, 1.0);

    return *this;
}

// http://www.codinglabs.net/article_world_view_projection_matrix.aspx
Matrix4 Transform::getMatrix() const
{
    Matrix4 m;

    m.setTransform(m_translation, m_rotation);

    // Composite
    return m;
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
    return Vector3(m[1][0], m[1][1], m[1][2]);
}

const Vector3& Transform::getRight()
{
    Matrix4 m = getMatrix();
    return Vector3(m[0][0], m[0][1], m[0][2]);
}

const Vector3& Transform::getUp()
{
    Matrix4 m = getMatrix();
    return Vector3(m[2][0], m[2][1], m[2][2]);
}

MINI_NAMESPACE_CLOSE