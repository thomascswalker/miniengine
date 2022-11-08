#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <string>
#include <format>
#include "api.h"
#include "maths.h"
#include "rotation.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

// Class for handling matrix data, particularly to support multiple
// indexing like `Matrix[0][0]`.
template <class T, int Rows, int Columns>
class MatrixData
{
public:
    T *operator[](int row)
    {
        int offset = row * Columns;
        return _data + offset;
    }

    T const *operator[](int row) const
    {
        int offset = row * Columns;
        return _data + offset;
    }

    T *getData()
    {
        return _data;
    }

    T const *getData() const
    {
        return _data;
    }

private:
    T _data[Rows * Columns];
};


// 4x4 Matrix
// Row-major
class Matrix4
{
public:
    // Constructors
    Matrix4() { setIdentity(); }
    Matrix4(const double m[4][4]) { set(m); }

    // Methods
    Matrix4&        set(double m00, double m01, double m02, double m03,
                        double m10, double m11, double m12, double m13,
                        double m20, double m21, double m22, double m23,
                        double m30, double m31, double m32, double m33);
    Matrix4&        set(const double m[4][4]);
    Matrix4&        setDiagonal(double v);
    Matrix4&        setIdentity();

    Matrix4&        setTransform(const Vector3 &t, const Rotation &r);
    Matrix4&        setTranslate(const Vector3 &t);
    Matrix4&        setTranslateOnly(const Vector3 &t);
    Matrix4&        setRotation(const Rotation& r);
    Matrix4&        setScale(double s);
    Matrix4&        setScale(const Vector3& s);

    Matrix4         getInverse(double* detPtr = NULL);
    Matrix4&        getTranspose();

    Vector3&        getTranslation() const;
    Rotation&       getRotation() const;

    std::string     toString();

    // Operators
    double*         operator [] (int i) { return m_mtx[i]; }
    const double*   operator [] (int i) const { return m_mtx[i]; }

    bool            operator == (const Matrix4& m) const;
    bool            operator != (const Matrix4& m) const;
    Matrix4&        operator *= (double v);
    Matrix4&        operator *= (const Matrix4& m);
    Matrix4&        operator += (const Matrix4& m);
    
    friend inline Matrix4  operator * (const Matrix4& m1, const Matrix4& m2)
    {
        Matrix4 tmp(m1);
        tmp *= m2;
        return tmp;
    }

    friend inline Vector3 operator *(const Matrix4& m, const Vector3& v)
    {
        double x, y, z, w;

        x = v.x() * m.m_mtx[0][0] + v.y() * m.m_mtx[0][1] + v.z() * m.m_mtx[0][2];
        y = v.x() * m.m_mtx[1][0] + v.y() * m.m_mtx[1][1] + v.z() * m.m_mtx[1][2];
        z = v.x() * m.m_mtx[2][0] + v.y() * m.m_mtx[2][1] + v.z() * m.m_mtx[2][2];
        w = v.x() * m.m_mtx[3][0] + v.y() * m.m_mtx[3][1] + v.z() * m.m_mtx[3][2];

        if (w != 0.0)
        {
            x /= w;
            y /= w;
            z /= w;
        }

        return Vector3(x, y, z);
    }

    // https://mathinsight.org/matrix_vector_multiplication
    friend inline Vector4 operator *(const Matrix4& m, const Vector4& v)
    {
        double x, y, z, w;

        x = v.x() * m.m_mtx[0][0] + v.y() * m.m_mtx[0][1] + v.z() * m.m_mtx[0][2] + v.w() * m.m_mtx[0][3];
        y = v.x() * m.m_mtx[1][0] + v.y() * m.m_mtx[1][1] + v.z() * m.m_mtx[1][2] + v.w() * m.m_mtx[1][3];
        z = v.x() * m.m_mtx[2][0] + v.y() * m.m_mtx[2][1] + v.z() * m.m_mtx[2][2] + v.w() * m.m_mtx[2][3];
        w = v.x() * m.m_mtx[3][0] + v.y() * m.m_mtx[3][1] + v.z() * m.m_mtx[3][2] + v.w() * m.m_mtx[3][3];

        if (w != 0.0)
        {
            x /= w;
            y /= w;
            z /= w;
        }

        return Vector4(x, y, z, w);
    }


private:
    MatrixData<double, 4, 4> m_mtx;

    Matrix4 _setRotateOnly(Quaternion &rot);
    void    _setRotateFromQuat(double r, const Vector3& i);
};

Matrix4 lookAt(const Vector3 eye, const Vector3 at, const Vector3 up);
Matrix4 makeTranslate(const Vector3& translation);
Matrix4 makeRotationX(double x);
Matrix4 makeRotationY(double y);
Matrix4 makeRotationZ(double z);
Matrix4 makeRotation(double x, double y, double z);
Matrix4 makeRotation(const Rotation& rotation);
Matrix4 makeScale(const Vector3& scale);

MINI_NAMESPACE_CLOSE

#endif