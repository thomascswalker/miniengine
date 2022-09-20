#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <string>
#include <format>
#include "math.h"
#include "rotation.h"

// Class for handling matrix data, particularly to support multiple
// indexing like `Matrix[0][0]`.
template <class T, int Rows, int Columns>
class MatrixData
{
public:

    /// Return a pointer to a \a row of data.
    T *operator[](int row)
    {
        int offset = row * Columns;
        return _data + offset;
    }

    /// Return a const pointer to a \a row of data.
    T const *operator[](int row) const
    {
        int offset = row * Columns;
        return _data + offset;
    }

    /// Return a pointer to the start of all the data.
    T *getData()
    {
        return _data;
    }

    /// Return a const pointer to the start of all the data.
    T const *getData() const
    {
        return _data;
    }

private:

    T _data[Rows * Columns];
};


// 4x4 Matrix
// [ 0, 0, 0, 0 ]
// [ 0, 0, 0, 0 ]
// [ 0, 0, 0, 0 ]
// [ 0, 0, 0, 0 ]
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
    Matrix4&        setTranslate(const Vector3 &t);
    Matrix4&        setRotation(const Rotation& r);
    Matrix4&        setScale(double scale);
    Matrix4&        setScale(const Vector3& scale);

    Matrix4         getInverse(double* detPtr = NULL);
    Vector3&        getTranslation() const;
    

    std::string     toString();

    // Operators
    double*         operator [] (int i) { return m_mtx[i]; }
    const double*   operator [] (int i) const { return m_mtx[i]; }

    bool            operator == (const Matrix4& m) const;
    bool            operator != (const Matrix4& m) const;
    Matrix4&        operator *= (double v);
    Matrix4&        operator += (const Matrix4& m);

    Matrix4&        operator *=(const Matrix4& m);
    friend Matrix4& operator * (const Matrix4& m1, const Matrix4& m2)
    {
        Matrix4 tmp(m1);
        tmp *= m2;
        return tmp;
    }
    friend inline Vector4 operator *(const Matrix4& m, const Vector4& vec)
    {
        return Vector4(vec[0] * m.m_mtx[0][0] + vec[1] * m.m_mtx[0][1] + vec[2] * m.m_mtx[0][2] + vec[3] * m.m_mtx[0][3],
                       vec[0] * m.m_mtx[1][0] + vec[1] * m.m_mtx[1][1] + vec[2] * m.m_mtx[1][2] + vec[3] * m.m_mtx[1][3],
                       vec[0] * m.m_mtx[2][0] + vec[1] * m.m_mtx[2][1] + vec[2] * m.m_mtx[2][2] + vec[3] * m.m_mtx[2][3],
                       vec[0] * m.m_mtx[3][0] + vec[1] * m.m_mtx[3][1] + vec[2] * m.m_mtx[3][2] + vec[3] * m.m_mtx[3][3]);
    }


private:
    MatrixData<double, 4, 4> m_mtx;

    Matrix4 _setRotateOnly(Quaternion &rot);
    void    _setRotateFromQuat(float r, const Vector3& i);
};

Matrix4 lookAt(const Vector3 eye, const Vector3 at, const Vector3 up);

//class Matrix3
//{
//public:
//    Matrix3();
//    Matrix3(const float values[9]);
//
//    Matrix3 identity();
//
//    float operator[](int index) const;
//    Matrix3 operator+(Matrix3& other) const
//    {
//        float values[9] = {
//            m_values[0] + other[0], m_values[1] + other[1], m_values[2] + other[2],
//            m_values[3] + other[3], m_values[4] + other[4], m_values[5] + other[5],
//            m_values[6] + other[6], m_values[7] + other[7], m_values[8] + other[8]
//        };
//        return Matrix3(values);
//    }
//    Matrix3 operator*(Matrix3& other) const
//    {
//        float values[9] = {
//            m_values[0] * other[0], m_values[1] * other[1], m_values[2] * other[2],
//            m_values[3] * other[3], m_values[4] * other[4], m_values[5] * other[5],
//            m_values[6] * other[6], m_values[7] * other[7], m_values[8] * other[8]
//        };
//        return Matrix3(values);
//    }
//
//private:
//    float m_values[9] = { 0,0,0,
//                            0,0,0,
//                            0,0,0 };
//};

//class Matrix4
//{
//public:
//    Matrix4();
//    Matrix4(const float values[16]);
//
//    Matrix4 identity();
//    Matrix4& setTranslate(int x, int y, int z);
//    Matrix4& setTranslate(const Vector3& vector);
//
//    Matrix4& rotateX(float angle);
//    Matrix4& rotateY(float angle);
//    Matrix4& rotateZ(float angle);
//
//    void setIndex(int index, float value);
//    void setRow(int index, Vector3& vector);
//    void setColumn(int index, Vector3& vector);
//    void setDiagonal(float value);
//
//    float operator[](int i) { return m_values[i]; }
//    const float operator [](int i) const { return m_values[i]; }
//    Matrix4 operator+(Matrix4& other) const
//    {
//        float values[16] = {
//            m_values[0] + other[0],  m_values[1] + other[1],  m_values[2] + other[2],  m_values[3] + other[3],
//            m_values[4] + other[4],  m_values[5] + other[5],  m_values[6] + other[6],  m_values[7] + other[7],
//            m_values[8] + other[8],  m_values[9] + other[9],  m_values[10] + other[10], m_values[11] + other[11],
//            m_values[12] + other[12], m_values[13] + other[13], m_values[14] + other[14], m_values[15] + other[15]
//        };
//        return Matrix4(values);
//    }
//    Matrix4 operator*(Matrix4& other) const
//    {
//        float values[16] = {
//            m_values[0] * other[0],  m_values[1] * other[1],  m_values[2] * other[2],  m_values[3] * other[3],
//            m_values[4] * other[4],  m_values[5] * other[5],  m_values[6] * other[6],  m_values[7] * other[7],
//            m_values[8] * other[8],  m_values[9] * other[9],  m_values[10] * other[10], m_values[11] * other[11],
//            m_values[12] * other[12], m_values[13] * other[13], m_values[14] * other[14], m_values[15] * other[15]
//        };
//        return Matrix4(values);
//    }
//    friend Matrix4 operator*(const Matrix4& m1, const Matrix4& m2)
//    {
//        Matrix4 tmp(m1);
//        tmp = tmp * m2;
//        return tmp;
//    }
//    Vector3 operator*(Vector3& in) const
//    {
//        //out = in * Mproj;
//        Vector3 out;
//
//        float x = in.x() * m_values[0]  + in.y() * m_values[1]  + in.z() * m_values[2]  + m_values[3];
//        float y = in.x() * m_values[4]  + in.y() * m_values[5]  + in.z() * m_values[6]  + m_values[7];
//        float z = in.x() * m_values[8]  + in.y() * m_values[9]  + in.z() * m_values[10] + m_values[11];
//        float w = in.x() * m_values[12] + in.y() * m_values[13] + in.z() * m_values[14] + m_values[15];
// 
//        // normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)
//        if (w != 1)
//        { 
//            x /= w;
//            y /= w;
//            z /= w;
//        }
//
//        return Vector3(x, y, z);
//    }
//
//private:
//    float m_values[16] = { 0,0,0,0,
//                            0,0,0,0,
//                            0,0,0,0,
//                            0,0,0,0 };
//};
//
//Matrix4& lookAt(const Vector3& origin, Vector3& target, const Vector3& up, Matrix4& m);
//bool factor(Matrix4* shearRotation, Vector3* scale, Matrix4* rotation, Vector3* translation, Matrix4* projection, double eps = 1e-10);


#endif