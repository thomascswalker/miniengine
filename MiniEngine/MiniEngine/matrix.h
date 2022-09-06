#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include "math.h"
#include "vector.h"

// Class for handling matrix data, particularly to support multiple
// indexing like `Matrix[0][0]`.
template <class T, int rows, int columns>
class MatrixData
{
public:
    T* operator [] (int row)
    {
        return m_data + (row * columns);
    }

    T* const operator [] (int row) const
    {
        return m_data + (row * columns);
    }

    T* getData()
    {
        return m_data;
    }

    T* const getData() const
    {
        return m_data;
    }


private:
    T m_data[rows, columns];
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
    Matrix4() = default;
    Matrix4(const double m[4][4]) { set(m); }

    // Methods
    Matrix4&        set(double m00, double m01, double m02, double m03,
                        double m10, double m11, double m12, double m13,
                        double m20, double m21, double m22, double m23,
                        double m30, double m31, double m32, double m33);
    Matrix4&        set(const double m[4][4]);
    Matrix4&        setDiagonal(double v);
    Matrix4&        setIdentity();
    Matrix4&        setScale(double scale);

    Matrix4         getInverse(double* detPtr = NULL);

    // Operators
    double*         operator [] (int i) { return m_mtx[i]; }
    const double*   operator [] (int i) const { return m_mtx[i]; }

    bool            operator == (const Matrix4& m) const;
    bool            operator != (const Matrix4& m) const;
    Matrix4&        operator *= (double v);
    Matrix4&        operator += (const Matrix4& m);

private:
    MatrixData<double, 4, 4> m_mtx;
};

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