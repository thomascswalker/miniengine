#include "matrix.h"

namespace Graphics {
using namespace Graphics;

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
    m_mtx[0][0] = m[0][0]; m_mtx[0][1] = m[0][1]; m_mtx[0][2] = m[0][1]; m_mtx[0][3] = m[0][1];
    m_mtx[1][0] = m[1][0]; m_mtx[1][1] = m[1][1]; m_mtx[1][2] = m[1][1]; m_mtx[1][3] = m[1][1];
    m_mtx[2][0] = m[2][0]; m_mtx[2][1] = m[2][1]; m_mtx[2][2] = m[2][1]; m_mtx[2][3] = m[2][1];
    m_mtx[3][0] = m[3][0]; m_mtx[3][1] = m[3][1]; m_mtx[3][2] = m[3][1]; m_mtx[3][3] = m[3][1];

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

Matrix4& Matrix4::setTransform(const Vector3& t, const Rotation& r)
{
    setIdentity();
    setRotation(r);
    setTranslateOnly(t);
    return *this;
}

Vector3 Matrix4::getTranslation() const
{
    auto t = Vector3(m_mtx[0][3], m_mtx[1][3], m_mtx[2][3]);
    return t;
}

Matrix4& Matrix4::setTranslate(const Vector3& t)
{
    setIdentity();
    m_mtx[0][3] = t._x;
    m_mtx[1][3] = t._y;
    m_mtx[2][3] = t._z;
    return *this;
}

Matrix4& Matrix4::setTranslateOnly(const Vector3& t)
{
    m_mtx[0][3] = t._x;
    m_mtx[1][3] = t._y;
    m_mtx[2][3] = t._z;
    return *this;
}

Matrix4& Matrix4::setRotation(const Rotation& r)
{
    auto quat = r.getQuaternion();
    _setRotateFromQuat(quat.getReal(), quat.getImaginary());

    m_mtx[3][0] = 0.0;
    m_mtx[3][1] = 0.0;
    m_mtx[3][2] = 0.0;

    m_mtx[0][3] = 0.0;
    m_mtx[1][3] = 0.0;
    m_mtx[2][3] = 0.0;
    m_mtx[3][3] = 1.0;

    return *this;
}

Matrix4& Matrix4::setScale(double s)
{
    setIdentity();
    m_mtx[0][0] = s;
    m_mtx[1][1] = s;
    m_mtx[2][2] = s;
    return *this;
}

Matrix4& Matrix4::setScale(const Vector3& s)
{
    setIdentity();
    m_mtx[0][0] = s._x;
    m_mtx[1][1] = s._y;
    m_mtx[2][2] = s._z;
    return *this;
}

/// Derived from https://github.com/PixarAnimationStudios/USD/blob/release/pxr/base/gf/matrix4d.cpp
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

    if (abs(det) > EPSILON)
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
	    inverse.setScale(CC_MAX);
    }

    return inverse;
}

Matrix4 Matrix4::getTranspose()
{
    Matrix4 m;

    m[0][0] = m_mtx[0][0];
    m[1][0] = m_mtx[0][1];
    m[2][0] = m_mtx[0][2];
    m[3][0] = m_mtx[0][3];
    m[0][1] = m_mtx[1][0];
    m[1][1] = m_mtx[1][1];
    m[2][1] = m_mtx[1][2];
    m[3][1] = m_mtx[1][3];
    m[0][2] = m_mtx[2][0];
    m[1][2] = m_mtx[2][1];
    m[2][2] = m_mtx[2][2];
    m[3][2] = m_mtx[2][3];
    m[0][3] = m_mtx[3][0];
    m[1][3] = m_mtx[3][1];
    m[2][3] = m_mtx[3][2];
    m[3][3] = m_mtx[3][3];

    return m;
}

Rotation Matrix4::getRotation() const
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

        imaginary._x = q;
        imaginary._y = (m_mtx[i][j] + m_mtx[j][i]) / (4.0 * q);
        imaginary._z = (m_mtx[k][i] + m_mtx[i][k]) / (4.0 * q);
	    real         = (m_mtx[j][k] - m_mtx[k][j]) / (4.0 * q);
    }

    Quaternion quat(clamp(real, -1.0, 1.0), imaginary);
    auto q = Rotation(quat);
    return q;
}

std::string Matrix4::toString()
{
    std::string string;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            double value = m_mtx[y][x];
            string += std::to_string(value) + ",";
        }
    }
    return string;
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

Matrix4& Matrix4::operator *= (const Matrix4& m)
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

Matrix4 lookAt(const Vector3 eye, const Vector3 at, const Vector3 up)
{
    Vector3 forward = at - eye;
    forward.normalize();

    Vector3 right = cross(forward, up);
    right.normalize();

    Vector3 newUp = cross(right, forward);

    Matrix4 view;
    view.set( right._x,   right._y,   right._z,   -dot(right, eye),
              newUp._x,   newUp._y,   newUp._z,   -dot(newUp, eye),
              forward._x, forward._y, forward._z, -dot(forward, eye),
              0.0,        0.0,        0.0,        1.0);

    return view;
}

Matrix4 makeTranslate(const Vector3& translation)
{
    Matrix4 t;
    t.setIdentity();
    t[3][0] = translation._x;
    t[3][1] = translation._y;
    t[3][2] = translation._z;
    return t;
}

Matrix4 makeRotationX(double x)
{
    Matrix4 m = Matrix4();
    m.setIdentity();

    double c = cos(x);
    double s = sin(x);

    m[1][1] = c;
    m[1][2] = -s;
    m[2][1] = s;
    m[2][2] = c;

    return m;
}

Matrix4 makeRotationY(double y)
{
    Matrix4 m = Matrix4();
    m.setIdentity();

    double c = cos(y);
    double s = sin(y);

    m[0][0] = c;
    m[0][2] = s;
    m[2][0] = -s;
    m[2][2] = c;

    return m;
}

Matrix4 makeRotationZ(double z)
{
    Matrix4 m = Matrix4();
    m.setIdentity();

    double c = cos(z);
    double s = sin(z);

    m[0][0] = c;
    m[0][1] = -s;
    m[1][0] = s;
    m[0][1] = c;

    return m;
}

Matrix4 makeRotation(double x, double y, double z)
{
    double cr = cos(x * 0.5);
    double sr = sin(x * 0.5);
    double cp = cos(y * 0.5);
    double sp = sin(y * 0.5);
    double cy = cos(z * 0.5);
    double sy = sin(z * 0.5);

    
    double angle = cr * cp * cy + sr * sp * sy;
    double _x = sr * cp * cy - cr * sp * sy;
    double _y = cr * sp * cy + sr * cp * sy;
    double _z = cr * cp * sy - sr * sp * cy;

    Vector3 axis(_x, _y, _z);

    double c = cos(angle);
    double s = sin(angle);

    Matrix4 m = Matrix4();

    m[0][0] = axis._x * axis._x * (1.0 - c) + c;
    m[0][1] = axis._y * axis._x * (1.0 - c) - s * axis._z;
    m[0][2] = axis._z * axis._x * (1.0 - c) + s * axis._y;

    m[1][0] = axis._x * axis._y * (1.0 - c) + s * axis._z;
    m[1][1] = axis._y * axis._y * (1.0 - c) + c;
    m[1][2] = axis._z * axis._y * (1.0 - c) - s * axis._x;

    m[2][0] = axis._x * axis._z * (1.0 - c) - s * axis._y;
    m[2][1] = axis._y * axis._z * (1.0 - c) + s * axis._x;
    m[2][2] = axis._z * axis._z * (1.0 - c) + c;

    return m;
}

Matrix4 makeRotation(const Rotation& rotation)
{
    Quaternion q = rotation.getQuaternion();

    Vector3 axis = q.getImaginary();
    axis.normalize();

    double angle = q.getReal();
    double c = cos(angle);
    double s = sin(angle);

    Matrix4 m = Matrix4();

    m[0][0] = axis._x * axis._x * (1.0 - c) + c;
    m[0][1] = axis._y * axis._x * (1.0 - c) - s * axis._z;
    m[0][2] = axis._z * axis._x * (1.0 - c) + s * axis._y;

    m[1][0] = axis._x * axis._y * (1.0 - c) + s * axis._z;
    m[1][1] = axis._y * axis._y * (1.0 - c) + c;
    m[1][2] = axis._z * axis._y * (1.0 - c) - s * axis._x;

    m[2][0] = axis._x * axis._z * (1.0 - c) - s * axis._y;
    m[2][1] = axis._y * axis._z * (1.0 - c) + s * axis._x;
    m[2][2] = axis._z * axis._z * (1.0 - c) + c;

    return m;
}

Matrix4 makeScale(const Vector3& scale)
{
    Matrix4 s;
    s.set(scale._x, 0.0,      0.0,      0.0,
          0.0,      scale._y, 0.0,      0.0,
          0.0,      0.0,      scale._z, 0.0,
          0.0,      0.0,      0.0,      1.0);
    return s;
}

}