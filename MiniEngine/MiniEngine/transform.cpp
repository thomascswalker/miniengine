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

Matrix4 Transform::getMatrix() const
{
    Matrix4 temp;

    // Pivot
    temp.setTranslate(-m_pivotPosition);

    // Scale
    temp.setRotation(m_pivotOrientation.getInverse());
    temp.setScale(m_scale);
    temp.setRotation(m_pivotOrientation);

    // Rotation
    temp.setRotation(m_rotation);

    // Pivot (again)
    temp.setTranslate(m_pivotPosition);

    // Translation
    temp.setTranslate(m_translation);

    return temp;
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

const Vector3& Transform::getFront()
{
    Vector3 front;
    Vector3 axis = m_rotation.getAxis();
    double x = cos(Math::degreesToRadians(axis.x())) * cos(Math::degreesToRadians(axis.x()));
    double y = sin(Math::degreesToRadians(axis.z()));
    double z = sin(Math::degreesToRadians(axis.x())) * cos(Math::degreesToRadians(axis.x()));
    
    front.setX(x);
    front.setY(y);
    front.setZ(z);
    front.normalize();

    return front;
}