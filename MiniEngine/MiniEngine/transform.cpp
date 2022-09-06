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
    Matrix4 mPivotPos, mPivotPosInv;
    mPivotPos.setTranslate(m_pivotPosition);
    mPivotPosInv.setTranslate(-m_pivotPosition);

    Matrix4 mNoPivot = mPivotPos * m * mPivotPosInv;
    Matrix4 shearRotMat, rotMat, projMat;
}
