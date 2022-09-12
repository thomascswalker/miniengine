#include "matrix.h"

Matrix4& Matrix4::set(double m00, double m01, double m02, double m03,
                      double m10, double m11, double m12, double m13,
                      double m20, double m21, double m22, double m23,
                      double m30, double m31, double m32, double m33)
{
    m_mtx[0][0] = m00; m_mtx[0][1] = m01; m_mtx[0][2] = m02; m_mtx[0][3] = m03;
    m_mtx[1][0] = m10; m_mtx[1][1] = m11; m_mtx[1][2] = m12; m_mtx[1][3] = m13;
    m_mtx[2][0] = m20; m_mtx[2][1] = m21; m_mtx[2][2] = m22; m_mtx[2][3] = m23;
    m_mtx[3][0] = m30; m_mtx[3][1] = m31; m_mtx[3][2] = m32; m_mtx[3][3] = m33;

    return *this;
}

Matrix4& Matrix4::set(const double m[4][4])
{
    m_mtx[0][0] = m[0][0]; m_mtx[0][1] = m[0][1]; m_mtx[0][2] = m[0][2]; m_mtx[0][3] = m[0][3];
    m_mtx[1][0] = m[1][0]; m_mtx[1][1] = m[1][1]; m_mtx[1][2] = m[1][2]; m_mtx[1][3] = m[1][3];
    m_mtx[2][0] = m[2][0]; m_mtx[2][1] = m[2][1]; m_mtx[2][2] = m[2][2]; m_mtx[2][3] = m[2][3];
    m_mtx[3][0] = m[3][0]; m_mtx[3][1] = m[3][1]; m_mtx[3][2] = m[3][2]; m_mtx[3][3] = m[3][3];

    return *this;
}

Matrix4& Matrix4::setDiagonal(double v)
{
    m_mtx[0][0] = v; m_mtx[0][1] = 0; m_mtx[0][2] = 0; m_mtx[0][3] = 0;
    m_mtx[1][0] = 0; m_mtx[1][1] = v; m_mtx[1][2] = 0; m_mtx[1][3] = 0;
    m_mtx[2][0] = 0; m_mtx[2][1] = 0; m_mtx[2][2] = v; m_mtx[2][3] = 0;
    m_mtx[3][0] = 0; m_mtx[3][1] = 0; m_mtx[3][2] = 0; m_mtx[3][3] = v;

    return *this;
}

Matrix4& Matrix4::setIdentity()
{
    setDiagonal(1.0);
    return (*this);
}

Matrix4& Matrix4::setTranslate(const Vector3& t)
{
    m_mtx[0][0] = 1.0; m_mtx[0][1] = 0.0;   m_mtx[0][2] = 0.0;   m_mtx[0][3] = 0.0;
    m_mtx[1][0] = 0.0;   m_mtx[1][1] = 1.0; m_mtx[1][2] = 0.0;   m_mtx[1][3] = 0.0;
    m_mtx[2][0] = 0.0;   m_mtx[2][1] = 0.0;   m_mtx[2][2] = 1.0; m_mtx[2][3] = 0.0;
    m_mtx[3][0] = t[0];   m_mtx[3][1] = t[1];   m_mtx[3][2] = t[2];   m_mtx[3][3] = 1.0;

    return *this;
}

Matrix4& Matrix4::setRotation(const Rotation& r)
{
    auto quat = r.getQuaternion();
    _setRotateFromQuat(quat.getReal(), quat.getImaginary());

    m_mtx[0][3] = 0.0;
    m_mtx[1][3] = 0.0;
    m_mtx[2][3] = 0.0;

    m_mtx[3][0] = 0.0;
    m_mtx[3][1] = 0.0;
    m_mtx[3][2] = 0.0;
    m_mtx[3][3] = 1.0;

    return *this;
}

Matrix4& Matrix4::setScale(double scale)
{
    m_mtx[0][0] = scale; m_mtx[0][1] = 0.0;   m_mtx[0][2] = 0.0;   m_mtx[0][3] = 0.0;
    m_mtx[1][0] = 0.0;   m_mtx[1][1] = scale; m_mtx[1][2] = 0.0;   m_mtx[1][3] = 0.0;
    m_mtx[2][0] = 0.0;   m_mtx[2][1] = 0.0;   m_mtx[2][2] = scale; m_mtx[2][3] = 0.0;
    m_mtx[3][0] = 0.0;   m_mtx[3][1] = 0.0;   m_mtx[3][2] = 0.0;   m_mtx[3][3] = 1.0;

    return *this;
}

Matrix4& Matrix4::setScale(const Vector3& scale)
{
    m_mtx[0][0] = scale[0]; m_mtx[0][1] = 0.0;      m_mtx[0][2] = 0.0;      m_mtx[0][3] = 0.0;
    m_mtx[1][0] = 0.0;      m_mtx[1][1] = scale[1]; m_mtx[1][2] = 0.0;      m_mtx[1][3] = 0.0;
    m_mtx[2][0] = 0.0;      m_mtx[2][1] = 0.0;      m_mtx[2][2] = scale[2]; m_mtx[2][3] = 0.0;
    m_mtx[3][0] = 0.0;      m_mtx[3][1] = 0.0;      m_mtx[3][2] = 0.0;      m_mtx[3][3] = 1.0;

    return *this;
}

/// Derived from https://github.com/PixarAnimationStudios/USD/blob/release/pxr/base/gf/matrix4d.cpp
/// Returns the inverse of the matrix, or FLT_MAX * SetIdentity() if the
/// matrix is singular. (FLT_MAX is the largest value a \c float can have,
/// as defined by the system.) The matrix is considered singular if the
/// determinant is less than or equal to the optional parameter \e eps. If
/// \e det is non-null, <c>*det</c> is set to the determinant.
Matrix4 Matrix4::getInverse(double* detPtr)
{
    double x00, x01, x02, x03;
    double x10, x11, x12, x13;
    double x20, x21, x22, x23;
    double x30, x31, x32, x33;
    double y01, y02, y03, y12, y13, y23;
    double z00, z10, z20, z30;
    double z01, z11, z21, z31;
    double z02, z03, z12, z13, z22, z23, z32, z33;

    // Pickle 1st two columns of matrix into registers
    x00 = m_mtx[0][0];
    x01 = m_mtx[0][1];
    x10 = m_mtx[1][0];
    x11 = m_mtx[1][1];
    x20 = m_mtx[2][0];
    x21 = m_mtx[2][1];
    x30 = m_mtx[3][0];
    x31 = m_mtx[3][1];

    // Compute all six 2x2 determinants of 1st two columns
    y01 = x00*x11 - x10*x01;
    y02 = x00*x21 - x20*x01;
    y03 = x00*x31 - x30*x01;
    y12 = x10*x21 - x20*x11;
    y13 = x10*x31 - x30*x11;
    y23 = x20*x31 - x30*x21;

    // Pickle 2nd two columns of matrix into registers
    x02 = m_mtx[0][2];
    x03 = m_mtx[0][3];
    x12 = m_mtx[1][2];
    x13 = m_mtx[1][3];
    x22 = m_mtx[2][2];
    x23 = m_mtx[2][3];
    x32 = m_mtx[3][2];
    x33 = m_mtx[3][3];

    // Compute all 3x3 cofactors for 2nd two columns */
    z33 = x02*y12 - x12*y02 + x22*y01;
    z23 = x12*y03 - x32*y01 - x02*y13;
    z13 = x02*y23 - x22*y03 + x32*y02;
    z03 = x22*y13 - x32*y12 - x12*y23;
    z32 = x13*y02 - x23*y01 - x03*y12;
    z22 = x03*y13 - x13*y03 + x33*y01;
    z12 = x23*y03 - x33*y02 - x03*y23;
    z02 = x13*y23 - x23*y13 + x33*y12;

    // Compute all six 2x2 determinants of 2nd two columns
    y01 = x02*x13 - x12*x03;
    y02 = x02*x23 - x22*x03;
    y03 = x02*x33 - x32*x03;
    y12 = x12*x23 - x22*x13;
    y13 = x12*x33 - x32*x13;
    y23 = x22*x33 - x32*x23;

    // Compute all 3x3 cofactors for 1st two columns
    z30 = x11*y02 - x21*y01 - x01*y12;
    z20 = x01*y13 - x11*y03 + x31*y01;
    z10 = x21*y03 - x31*y02 - x01*y23;
    z00 = x11*y23 - x21*y13 + x31*y12;
    z31 = x00*y12 - x10*y02 + x20*y01;
    z21 = x10*y03 - x30*y01 - x00*y13;
    z11 = x00*y23 - x20*y03 + x30*y02;
    z01 = x20*y13 - x30*y12 - x10*y23;

    // compute 4x4 determinant & its reciprocal
    double det = x30*z30 + x20*z20 + x10*z10 + x00*z00;
    if (detPtr)
    {
	    *detPtr = det;
    }

    Matrix4 inverse;

    if (abs(det) > Math::EPSILON)
    {
        double rcp = 1.0 / det;

	    // Multiply all 3x3 cofactors by reciprocal & transpose
        inverse.m_mtx[0][0] = z00*rcp;
        inverse.m_mtx[0][1] = z10*rcp;
        inverse.m_mtx[1][0] = z01*rcp;
        inverse.m_mtx[0][2] = z20*rcp;
        inverse.m_mtx[2][0] = z02*rcp;
        inverse.m_mtx[0][3] = z30*rcp;
        inverse.m_mtx[3][0] = z03*rcp;
        inverse.m_mtx[1][1] = z11*rcp;
        inverse.m_mtx[1][2] = z21*rcp;
        inverse.m_mtx[2][1] = z12*rcp;
        inverse.m_mtx[1][3] = z31*rcp;
        inverse.m_mtx[3][1] = z13*rcp;
        inverse.m_mtx[2][2] = z22*rcp;
        inverse.m_mtx[2][3] = z32*rcp;
        inverse.m_mtx[3][2] = z23*rcp;
        inverse.m_mtx[3][3] = z33*rcp;

    }
    else
    {
	    inverse.setScale(FLT_MAX);
    }

    return inverse;
};

std::string Matrix4::toString()
{
    return std::format("[{}, {}, {}, {}]\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]\n",
                        m_mtx[0][0], m_mtx[0][1], m_mtx[0][2], m_mtx[0][3],
                        m_mtx[1][0], m_mtx[1][1], m_mtx[1][2], m_mtx[1][3],
                        m_mtx[2][0], m_mtx[2][1], m_mtx[2][2], m_mtx[2][3],
                        m_mtx[3][0], m_mtx[3][1], m_mtx[3][2], m_mtx[3][3]);
}

bool Matrix4::operator == (const Matrix4& m) const
{
    return (m_mtx[0][0] == m.m_mtx[0][0] &&
            m_mtx[0][1] == m.m_mtx[0][1] &&
            m_mtx[0][2] == m.m_mtx[0][2] &&
            m_mtx[0][3] == m.m_mtx[0][3] &&
            m_mtx[1][0] == m.m_mtx[1][0] &&
            m_mtx[1][1] == m.m_mtx[1][1] &&
            m_mtx[1][2] == m.m_mtx[1][2] &&
            m_mtx[1][3] == m.m_mtx[1][3] &&
            m_mtx[2][0] == m.m_mtx[2][0] &&
            m_mtx[2][1] == m.m_mtx[2][1] &&
            m_mtx[2][2] == m.m_mtx[2][2] &&
            m_mtx[2][3] == m.m_mtx[2][3] &&
            m_mtx[3][0] == m.m_mtx[3][0] &&
            m_mtx[3][1] == m.m_mtx[3][1] &&
            m_mtx[3][2] == m.m_mtx[3][2] &&
            m_mtx[3][3] == m.m_mtx[3][3]);
}

bool Matrix4::operator != (const Matrix4& m) const
{
    return !(*this == m);
}

Matrix4& Matrix4::operator *= (double v)
{
    m_mtx[0][0] *= v;
    m_mtx[0][1] *= v;
    m_mtx[0][2] *= v;
    m_mtx[0][3] *= v;
    m_mtx[1][0] *= v;
    m_mtx[1][1] *= v;
    m_mtx[1][2] *= v;
    m_mtx[1][3] *= v;
    m_mtx[2][0] *= v;
    m_mtx[2][1] *= v;
    m_mtx[2][2] *= v;
    m_mtx[2][3] *= v;
    m_mtx[3][0] *= v;
    m_mtx[3][1] *= v;
    m_mtx[3][2] *= v;
    m_mtx[3][3] *= v;

    return *this;
}

Matrix4& Matrix4::operator += (const Matrix4& m)
{
    m_mtx[0][0] += m.m_mtx[0][0];
    m_mtx[0][1] += m.m_mtx[0][1];
    m_mtx[0][2] += m.m_mtx[0][2];
    m_mtx[0][3] += m.m_mtx[0][3];
    m_mtx[1][0] += m.m_mtx[1][0];
    m_mtx[1][1] += m.m_mtx[1][1];
    m_mtx[1][2] += m.m_mtx[1][2];
    m_mtx[1][3] += m.m_mtx[1][3];
    m_mtx[2][0] += m.m_mtx[2][0];
    m_mtx[2][1] += m.m_mtx[2][1];
    m_mtx[2][2] += m.m_mtx[2][2];
    m_mtx[2][3] += m.m_mtx[2][3];
    m_mtx[3][0] += m.m_mtx[3][0];
    m_mtx[3][1] += m.m_mtx[3][1];
    m_mtx[3][2] += m.m_mtx[3][2];
    m_mtx[3][3] += m.m_mtx[3][3];

    return *this;
}

Matrix4& Matrix4::operator*=(const Matrix4& m)
{
    // Save current values before they are overwritten
    Matrix4 tmp = *this;

    m_mtx[0][0] = tmp.m_mtx[0][0] * m.m_mtx[0][0] +
                 tmp.m_mtx[0][1] * m.m_mtx[1][0] +
                 tmp.m_mtx[0][2] * m.m_mtx[2][0] +
                 tmp.m_mtx[0][3] * m.m_mtx[3][0];

    m_mtx[0][1] = tmp.m_mtx[0][0] * m.m_mtx[0][1] +
                 tmp.m_mtx[0][1] * m.m_mtx[1][1] +
                 tmp.m_mtx[0][2] * m.m_mtx[2][1] +
                 tmp.m_mtx[0][3] * m.m_mtx[3][1];

    m_mtx[0][2] = tmp.m_mtx[0][0] * m.m_mtx[0][2] +
                 tmp.m_mtx[0][1] * m.m_mtx[1][2] +
                 tmp.m_mtx[0][2] * m.m_mtx[2][2] +
                 tmp.m_mtx[0][3] * m.m_mtx[3][2];

    m_mtx[0][3] = tmp.m_mtx[0][0] * m.m_mtx[0][3] +
                 tmp.m_mtx[0][1] * m.m_mtx[1][3] +
                 tmp.m_mtx[0][2] * m.m_mtx[2][3] +
                 tmp.m_mtx[0][3] * m.m_mtx[3][3];

    m_mtx[1][0] = tmp.m_mtx[1][0] * m.m_mtx[0][0] +
                 tmp.m_mtx[1][1] * m.m_mtx[1][0] +
                 tmp.m_mtx[1][2] * m.m_mtx[2][0] +
                 tmp.m_mtx[1][3] * m.m_mtx[3][0];

    m_mtx[1][1] = tmp.m_mtx[1][0] * m.m_mtx[0][1] +
                 tmp.m_mtx[1][1] * m.m_mtx[1][1] +
                 tmp.m_mtx[1][2] * m.m_mtx[2][1] +
                 tmp.m_mtx[1][3] * m.m_mtx[3][1];

    m_mtx[1][2] = tmp.m_mtx[1][0] * m.m_mtx[0][2] +
                 tmp.m_mtx[1][1] * m.m_mtx[1][2] +
                 tmp.m_mtx[1][2] * m.m_mtx[2][2] +
                 tmp.m_mtx[1][3] * m.m_mtx[3][2];

    m_mtx[1][3] = tmp.m_mtx[1][0] * m.m_mtx[0][3] +
                 tmp.m_mtx[1][1] * m.m_mtx[1][3] +
                 tmp.m_mtx[1][2] * m.m_mtx[2][3] +
                 tmp.m_mtx[1][3] * m.m_mtx[3][3];

    m_mtx[2][0] = tmp.m_mtx[2][0] * m.m_mtx[0][0] +
                 tmp.m_mtx[2][1] * m.m_mtx[1][0] +
                 tmp.m_mtx[2][2] * m.m_mtx[2][0] +
                 tmp.m_mtx[2][3] * m.m_mtx[3][0];

    m_mtx[2][1] = tmp.m_mtx[2][0] * m.m_mtx[0][1] +
                 tmp.m_mtx[2][1] * m.m_mtx[1][1] +
                 tmp.m_mtx[2][2] * m.m_mtx[2][1] +
                 tmp.m_mtx[2][3] * m.m_mtx[3][1];

    m_mtx[2][2] = tmp.m_mtx[2][0] * m.m_mtx[0][2] +
                 tmp.m_mtx[2][1] * m.m_mtx[1][2] +
                 tmp.m_mtx[2][2] * m.m_mtx[2][2] +
                 tmp.m_mtx[2][3] * m.m_mtx[3][2];

    m_mtx[2][3] = tmp.m_mtx[2][0] * m.m_mtx[0][3] +
                 tmp.m_mtx[2][1] * m.m_mtx[1][3] +
                 tmp.m_mtx[2][2] * m.m_mtx[2][3] +
                 tmp.m_mtx[2][3] * m.m_mtx[3][3];

    m_mtx[3][0] = tmp.m_mtx[3][0] * m.m_mtx[0][0] +
                 tmp.m_mtx[3][1] * m.m_mtx[1][0] +
                 tmp.m_mtx[3][2] * m.m_mtx[2][0] +
                 tmp.m_mtx[3][3] * m.m_mtx[3][0];

    m_mtx[3][1] = tmp.m_mtx[3][0] * m.m_mtx[0][1] +
                 tmp.m_mtx[3][1] * m.m_mtx[1][1] +
                 tmp.m_mtx[3][2] * m.m_mtx[2][1] +
                 tmp.m_mtx[3][3] * m.m_mtx[3][1];

    m_mtx[3][2] = tmp.m_mtx[3][0] * m.m_mtx[0][2] +
                 tmp.m_mtx[3][1] * m.m_mtx[1][2] +
                 tmp.m_mtx[3][2] * m.m_mtx[2][2] +
                 tmp.m_mtx[3][3] * m.m_mtx[3][2];

    m_mtx[3][3] = tmp.m_mtx[3][0] * m.m_mtx[0][3] +
                 tmp.m_mtx[3][1] * m.m_mtx[1][3] +
                 tmp.m_mtx[3][2] * m.m_mtx[2][3] +
                 tmp.m_mtx[3][3] * m.m_mtx[3][3];

    return *this;
}

// Private //
Matrix4 Matrix4::_setRotateOnly(Quaternion& rot)
{
    Matrix4::_setRotateFromQuat(rot.getReal(), rot.getImaginary());
    return (*this);
}

void Matrix4::_setRotateFromQuat(float r, const Vector3& i)
{
    m_mtx[0][0] = 1.0 - 2.0 * (i[1] * i[1] + i[2] * i[2]);
    m_mtx[0][1] =       2.0 * (i[0] * i[1] + i[2] *    r);
    m_mtx[0][2] =       2.0 * (i[2] * i[0] - i[1] *    r);

    m_mtx[1][0] =       2.0 * (i[0] * i[1] - i[2] *    r);
    m_mtx[1][1] = 1.0 - 2.0 * (i[2] * i[2] + i[0] * i[0]);
    m_mtx[1][2] =       2.0 * (i[1] * i[2] + i[0] *    r);

    m_mtx[2][0] =       2.0 * (i[2] * i[0] + i[1] *    r);
    m_mtx[2][1] =       2.0 * (i[1] * i[2] - i[0] *    r);
    m_mtx[2][2] = 1.0 - 2.0 * (i[1] * i[1] + i[0] * i[0]);
}

//Matrix2::Matrix2()
//{
//    identity();
//}
//
//Matrix2::Matrix2(const float values[4])
//{
//    for (int i = 0; i < 4; i++)
//    {
//        m_values[i] = values[i];
//    }
//}
//
//Matrix2 Matrix2::identity()
//{
//    m_values[0] = 1.0f;
//    m_values[3] = 1.0f;
//
//    m_values[1] = 0.0f;
//    m_values[2] = 0.0f;
//
//    return *this;
//}
//
//Matrix3::Matrix3()
//{
//    identity();
//}
//
//Matrix3::Matrix3(const float values[9])
//{
//    for (int i = 0; i < 9; i++)
//    {
//        m_values[i] = values[i];
//    }
//}
//
//Matrix3 Matrix3::identity()
//{
//    m_values[0] = 1.0f;
//    m_values[4] = 1.0f;
//    m_values[8] = 1.0f;
//
//    m_values[1] = 0.0f;
//    m_values[2] = 0.0f;
//    m_values[3] = 0.0f;
//
//    m_values[5] = 0.0f;
//    m_values[6] = 0.0f;
//    m_values[7] = 0.0f;
//
//    return *this;
//}
//
//
//Matrix4::Matrix4()
//{
//    identity();
//}
//
//Matrix4::Matrix4(const float values[16])
//{
//    for (int i = 0; i < 16; i++)
//    {
//        m_values[i] = values[i];
//    }
//}
//
//Matrix4 Matrix4::identity()
//{
//    m_values[0] = 1.0f;
//    m_values[5] = 1.0f;
//    m_values[10] = 1.0f;
//    m_values[15] = 1.0f;
//
//    m_values[1] = 0.0f;
//    m_values[2] = 0.0f;
//    m_values[3] = 0.0f;
//    m_values[4] = 0.0f;
//
//    m_values[6] = 0.0f;
//    m_values[7] = 0.0f;
//    m_values[8] = 0.0f;
//    m_values[9] = 0.0f;
//
//    m_values[11] = 0.0f;
//    m_values[12] = 0.0f;
//    m_values[13] = 0.0f;
//    m_values[14] = 0.0f;
//
//    return *this;
//}
//
//Matrix4& Matrix4::setTranslate(int x, int y, int z)
//{
//    m_values[0]  = 1.0f; m_values[1]  = 0.0f; m_values[2]  = 0.0f; m_values[3]  = 0.0f;
//    m_values[4]  = 0.0f; m_values[5]  = 1.0f; m_values[6]  = 0.0f; m_values[7]  = 0.0f;
//    m_values[8]  = 0.0f; m_values[9]  = 0.0f; m_values[10] = 1.0f; m_values[11] = 0.0f;
//    m_values[12] = x;    m_values[13] = y;    m_values[14] = z;    m_values[15] = 1.0f;
//
//    return *this;
//}
//
//Matrix4& Matrix4::setTranslate(const Vector3& vector)
//{
//    return setTranslate(vector.x(), vector.y(), vector.z());
//}
//
//void Matrix4::setIndex(int index, float value)
//{
//    m_values[index] = value;
//}
//
//void Matrix4::setRow(int index, Vector3& vector)
//{
//    m_values[index] = vector.x();
//    m_values[index + 4] = vector.y();
//    m_values[index + 8] = vector.z();
//}
//
//void Matrix4::setColumn(int index, Vector3& vector)
//{
//    m_values[index * 4] = vector.x();
//    m_values[index * 4 + 1] = vector.y();
//    m_values[index * 4 + 2] = vector.z();
//}
//
//void Matrix4::setDiagonal(float value)
//{
//    m_values[0] = value;
//    m_values[5] = value;
//    m_values[10] = value;
//    m_values[15] = value;
//
//    m_values[1] = 0.0f;
//    m_values[2] = 0.0f;
//    m_values[3] = 0.0f;
//    m_values[4] = 0.0f;
//
//    m_values[6] = 0.0f;
//    m_values[7] = 0.0f;
//    m_values[8] = 0.0f;
//    m_values[9] = 0.0f;
//
//    m_values[11] = 0.0f;
//    m_values[12] = 0.0f;
//    m_values[13] = 0.0f;
//    m_values[14] = 0.0f;
//}
//
//Matrix4& Matrix4::rotateX(float angle)
//{
//    float c = cosf(angle);
//    float s = sinf(angle);
//
//    const float values[16] = { 1,  0,  0,  0,
//                                0,  c,  -s, 0,
//                                0,  s,  c,  0,
//                                0,  0,  0,  1 };
//
//    for (int i = 0; i < 16; i++)
//    {
//        m_values[i] = values[i];
//    }
//
//    return *this;
//}
//
//Matrix4& Matrix4::rotateY(float angle)
//{
//    float c = cosf(angle);
//    float s = sinf(angle);
//
//    const float values[16] = { c,  0, s, 0,
//                                0,  1, 0, 0,
//                                -s, 0, c, 0,
//                                0,  0, 0, 1 };
//
//    for (int i = 0; i < 16; i++)
//    {
//        m_values[i] = values[i];
//    }
//
//    return *this;
//}
//
//Matrix4& Matrix4::rotateZ(float angle)
//{
//    float c = cosf(angle);
//    float s = sinf(angle);
//
//    const float values[16] = { c,  s, 0, 0,
//                                -s, c,  0, 0,
//                                0,  0,  1, 0,
//                                0,  0,  0, 1 };
//
//    for (int i = 0; i < 16; i++)
//    {
//        m_values[i] = values[i];
//    }
//
//    return *this;
//}
//
//Matrix4& lookAt(const Vector3& origin, Vector3& target, const Vector3& up, Matrix4& m)
//{
//    Vector3 forward = origin - target;
//    forward.normalize();
//
//    Vector3 right = cross(origin, target);
//    right.normalize();
//
//    Vector3 newUp = cross(forward, right);
//
//    m.setIndex(0, right.x());
//    m.setIndex(1, right.y());
//    m.setIndex(2, right.z());
//
//    m.setIndex(5, newUp.x());
//    m.setIndex(6, newUp.y());
//    m.setIndex(7, newUp.z());
//
//    m.setIndex(9, forward.x());
//    m.setIndex(10, forward.y());
//    m.setIndex(11, forward.z());
//
//    return m;
//}
//    
///*
//        
//*/
//bool factor(
//    Matrix4* shearRotation,
//    Vector3* scale,
//    Matrix4* rotation,
//    Vector3* translation,
//    Matrix4* projection,
//    double eps)
//{
//    projection->setDiagonal(1.0f);
//}
//
//float Matrix2::operator[](int index) const
//{
//    return m_values[index];
//}
//
//float Matrix3::operator[](int index) const
//{
//    return m_values[index];
//}
