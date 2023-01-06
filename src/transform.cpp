#include "transform.h"

namespace Graphics
{
    using namespace Graphics;

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

    Vector3 Transform::getForward()
    {
        Matrix4 m = getMatrix();
        return { m[1][0], m[1][1], m[1][2] };
    }

    Vector3 Transform::getRight()
    {
        Matrix4 m = getMatrix();
        return { m[0][0], m[0][1], m[0][2] };
    }

    Vector3 Transform::getUp()
    {
        Matrix4 m = getMatrix();
        return { m[2][0], m[2][1], m[2][2] };
    }

}