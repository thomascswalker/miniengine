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
    m_mtx[0][0] = 1.0;  m_mtx[0][1] = 0.0;  m_mtx[0][2] = 0.0;  m_mtx[0][3] = 0.0;
    m_mtx[1][0] = 0.0;  m_mtx[1][1] = 1.0;  m_mtx[1][2] = 0.0;  m_mtx[1][3] = 0.0;
    m_mtx[2][0] = 0.0;  m_mtx[2][1] = 0.0;  m_mtx[2][2] = 1.0;  m_mtx[2][3] = 0.0;
    m_mtx[3][0] = t[0]; m_mtx[3][1] = t[1]; m_mtx[3][2] = t[2]; m_mtx[3][3] = 1.0;

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
    y01 = (x00 * x11) - (x10 * x01);
    y02 = (x00 * x21) - (x20 * x01);
    y03 = (x00 * x31) - (x30 * x01);
    y12 = (x10 * x21) - (x20 * x11);
    y13 = (x10 * x31) - (x30 * x11);
    y23 = (x20 * x31) - (x30 * x21);

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
    z33 = (x02 * y12) - (x12 * y02) + (x22 * y01);
    z23 = (x12 * y03) - (x32 * y01) - (x02 * y13);
    z13 = (x02 * y23) - (x22 * y03) + (x32 * y02);
    z03 = (x22 * y13) - (x32 * y12) - (x12 * y23);
    z32 = (x13 * y02) - (x23 * y01) - (x03 * y12);
    z22 = (x03 * y13) - (x13 * y03) + (x33 * y01);
    z12 = (x23 * y03) - (x33 * y02) - (x03 * y23);
    z02 = (x13 * y23) - (x23 * y13) + (x33 * y12);

    // Compute all six 2x2 determinants of 2nd two columns
    y01 = (x02 * x13) - (x12 * x03);
    y02 = (x02 * x23) - (x22 * x03);
    y03 = (x02 * x33) - (x32 * x03);
    y12 = (x12 * x23) - (x22 * x13);
    y13 = (x12 * x33) - (x32 * x13);
    y23 = (x22 * x33) - (x32 * x23);

    // Compute all 3x3 cofactors for 1st two columns
    z30 = (x11 * y02) - (x21 * y01) - (x01 * y12);
    z20 = (x01 * y13) - (x11 * y03) + (x31 * y01);
    z10 = (x21 * y03) - (x31 * y02) - (x01 * y23);
    z00 = (x11 * y23) - (x21 * y13) + (x31 * y12);
    z31 = (x00 * y12) - (x10 * y02) + (x20 * y01);
    z21 = (x10 * y03) - (x30 * y01) - (x00 * y13);
    z11 = (x00 * y23) - (x20 * y03) + (x30 * y02);
    z01 = (x20 * y13) - (x30 * y12) - (x10 * y23);

    // compute 4x4 determinant & its reciprocal
    double det = (x30 * z30) + (x20 * z20) + (x10 * z10) + (x00 * z00);
    if (detPtr)
    {
	    *detPtr = det;
    }

    Matrix4 inverse;

    if (abs(det) > Math::EPSILON)
    {
        double rcp = 1.0 / det;

	    // Multiply all 3x3 cofactors by reciprocal & transpose
        inverse.m_mtx[0][0] = z00 * rcp;
        inverse.m_mtx[0][1] = z10 * rcp;
        inverse.m_mtx[1][0] = z01 * rcp;
        inverse.m_mtx[0][2] = z20 * rcp;
        inverse.m_mtx[2][0] = z02 * rcp;
        inverse.m_mtx[0][3] = z30 * rcp;
        inverse.m_mtx[3][0] = z03 * rcp;
        inverse.m_mtx[1][1] = z11 * rcp;
        inverse.m_mtx[1][2] = z21 * rcp;
        inverse.m_mtx[2][1] = z12 * rcp;
        inverse.m_mtx[1][3] = z31 * rcp;
        inverse.m_mtx[3][1] = z13 * rcp;
        inverse.m_mtx[2][2] = z22 * rcp;
        inverse.m_mtx[2][3] = z32 * rcp;
        inverse.m_mtx[3][2] = z23 * rcp;
        inverse.m_mtx[3][3] = z33 * rcp;

    }
    else
    {
	    inverse.setScale(FLT_MAX);
    }

    return inverse;
};

Vector3& Matrix4::getTranslation() const
{
    auto t = Vector3(m_mtx[3][0], m_mtx[3][1], m_mtx[3][2]);
    return t;
}

Rotation& Matrix4::getRotation() const
{
    int i;

    if (m_mtx[0][0] > m_mtx[1][1])
    {
    	i = (m_mtx[0][0] > m_mtx[2][2] ? 0 : 2);
    }
    else
    {
        i = (m_mtx[1][1] > m_mtx[2][2] ? 1 : 2);
    }

    Vector3 imaginary;
    double  real;

    if (m_mtx[0][0] + m_mtx[1][1] + m_mtx[2][2] > m_mtx[i][i])
    {
	    real = 0.5 * sqrt(m_mtx[0][0] + m_mtx[1][1] + m_mtx[2][2] + m_mtx[3][3]);
	    imaginary.set((m_mtx[1][2] - m_mtx[2][1]) / (4.0 * real),
	                  (m_mtx[2][0] - m_mtx[0][2]) / (4.0 * real),
	                  (m_mtx[0][1] - m_mtx[1][0]) / (4.0 * real));
    }
    else
    {
	    int j = (i + 1) % 3;
	    int k = (i + 2) % 3;
	    double q = 0.5 * sqrt(m_mtx[i][i] - m_mtx[j][j] - m_mtx[k][k] + m_mtx[3][3]); 

        imaginary.setX(q);
        imaginary.setY((m_mtx[i][j] + m_mtx[j][i]) / (4 * q));
        imaginary.setZ((m_mtx[k][i] + m_mtx[i][k]) / (4 * q));
	    real         = (m_mtx[j][k] - m_mtx[k][j]) / (4 * q);
    }

    Quaternion quat(Math::clamp(real, -1.0, 1.0), imaginary);
    auto q = Rotation(quat);
    return q;
}

std::string Matrix4::toString()
{
    return std::format("[{:.2f}, {:.2f}, {:.2f}, {:.2f}]\n[{:.2f}, {:.2f}, {:.2f}, {:.2f}]\n[{:.2f}, {:.2f}, {:.2f}, {:.2f}]\n[{:.2f}, {:.2f}, {:.2f}, {:.2f}]\n",
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

void Matrix4::_setRotateFromQuat(double r, const Vector3& i)
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

/*
    https://www.geertarien.com/blog/2017/07/30/breakdown-of-the-lookAt-function-in-OpenGL/
*/
Matrix4 lookAt(const Vector3 eye, const Vector3 at, const Vector3 up)
{
    Vector3 zAxis = at - eye;
    zAxis.normalize();

    Vector3 xAxis = cross(zAxis, up);
    xAxis.normalize();

    Vector3 yAxis = cross(xAxis, zAxis);

    zAxis *= -1.0;

    Matrix4 viewMatrix;
    viewMatrix.set( xAxis.x(),  xAxis.y(),  xAxis.z(),  (double) -Math::dot(xAxis, eye),
                    yAxis.x(),  yAxis.y(),  yAxis.z(),  (double) -Math::dot(yAxis, eye),
                    zAxis.x(),  zAxis.y(),  zAxis.z(),  (double) -Math::dot(zAxis, eye),
                    eye.x(),    eye.y(),    eye.z(),    1.0);

    return viewMatrix;
}



Matrix4 makeViewport(double w, double h)
{
    Matrix4 view;
    view.set(w / 2.0,   0.0,        0.0,            w / 2.0,
             0.0,       h / 2.0,    0.0,            h / 2.0,
             0.0,       0.0,        255.0 / 2.0,    255.0 / 2.0,
             0.0,       0.0,        0.0,            1.0);
    return view;
}

Matrix4 makeTranslate(const Vector3& v)
{
    Matrix4 view;
    view.set(1.0,   0.0,    0.0,    v.x(),
             0.0,   1.0,    0.0,    v.y(),
             0.0,   0.0,    1.0,    v.z(),
             0.0,   0.0,    0.0,    1.0);
    return view;
}

Matrix4 makeRotation(const Vector3 rotation)
{
    float x = Math::degreesToRadians(rotation.x());
    float y = Math::degreesToRadians(rotation.y());
    float z = Math::degreesToRadians(rotation.z());

    Matrix4 rx;
    rx.set(
        1.0,   0.0,       0.0,     0.0,
        0.0,   cosf(x),    -sinf(x), 0.0,
        0.0,   sinf(x),    cosf(x),  0.0,
        0.0,   0.0,       0.0,     1.0
    );

    Matrix4 ry;
    ry.set(
        cosf(y), 0.0,      -sinf(y),   0.0,
        0.0,    1.0,      0.0,       0.0,
        sinf(y), 0.0,      cosf(y),    0.0,
        0.0,    0.0,      0.0,       1.0
    );

    Matrix4 rz;
    rz.set(
        cosf(z), -sinf(z),  0.0,       0.0,
        sinf(z), cosf(z),   0.0,       0.0,
        0.0,    0.0,      1.0,       0.0,
        0.0,    0.0,      0.0,       1.0
    );

    return rx * ry * rz;
}
