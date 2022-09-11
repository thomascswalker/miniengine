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

void Transform::setMatrix(const Matrix4& m)
{
    //Matrix4 mPivotPos, mPivotPosInv;
    //mPivotPos.setTranslate(m_pivotPosition);
    //mPivotPosInv.setTranslate(-m_pivotPosition);

    //Matrix4 mNoPivot = mPivotPos * m * mPivotPosInv;
    //Matrix4 shearRotMat, rotMat, projMat;
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