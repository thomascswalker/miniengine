#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include "vector.h"

namespace Matrices
{
    const static float EPSILON = 0.00001f;
    class Matrix2
    {
    public:
        Matrix2();
        Matrix2(const float values[4]);

        Matrix2 identity();

        float operator[](int index) const;
        Matrix2 operator+(Matrix2& other) const
        {
            float values[4] = {
                m_values[0] + other[0], m_values[1] + other[1],
                m_values[2] + other[2], m_values[3] + other[3]
            };
            return Matrix2(values);
        }
        Matrix2 operator*(Matrix2& other) const
        {
            float values[4] = {
                m_values[0] * other[0], m_values[1] * other[1],
                m_values[2] * other[2], m_values[3] * other[3]
            };
            return Matrix2(values);
        }

    private:
        float m_values[4] = { 0,0,
                              0,0 };
    };

    class Matrix3
    {
    public:
        Matrix3();
        Matrix3(const float values[9]);

        Matrix3 identity();

        float operator[](int index) const;
        Matrix3 operator+(Matrix3& other) const
        {
            float values[9] = {
                m_values[0] + other[0], m_values[1] + other[1], m_values[2] + other[2],
                m_values[3] + other[3], m_values[4] + other[4], m_values[5] + other[5],
                m_values[6] + other[6], m_values[7] + other[7], m_values[8] + other[8]
            };
            return Matrix3(values);
        }
        Matrix3 operator*(Matrix3& other) const
        {
            float values[9] = {
                m_values[0] * other[0], m_values[1] * other[1], m_values[2] * other[2],
                m_values[3] * other[3], m_values[4] * other[4], m_values[5] * other[5],
                m_values[6] * other[6], m_values[7] * other[7], m_values[8] * other[8]
            };
            return Matrix3(values);
        }

    private:
        float m_values[9] = { 0,0,0,
                              0,0,0,
                              0,0,0 };
    };

    class Matrix4
    {
    public:
        Matrix4();
        Matrix4(const float values[16]);

        Matrix4 identity();
        void translate(int x, int y, int z);

        Matrix4& lookAt(Vector3& target);

        Matrix4& rotateX(float angle);
        Matrix4& rotateY(float angle);
        Matrix4& rotateZ(float angle);

        void setRow(int index, Vector3& vector);
        void setColumn(int index, Vector3& vector);

        float operator[](int index) const;
        Matrix4 operator+(Matrix4& other) const
        {
            float values[16] = {
                m_values[0] + other[0],  m_values[1] + other[1],  m_values[2] + other[2],  m_values[3] + other[3],
                m_values[4] + other[4],  m_values[5] + other[5],  m_values[6] + other[6],  m_values[7] + other[7],
                m_values[8] + other[8],  m_values[9] + other[9],  m_values[10] + other[10], m_values[11] + other[11],
                m_values[12] + other[12], m_values[13] + other[13], m_values[14] + other[14], m_values[15] + other[15]
            };
            return Matrix4(values);
        }
        Matrix4 operator*(Matrix4& other) const
        {
            float values[16] = {
                m_values[0] * other[0],  m_values[1] * other[1],  m_values[2] * other[2],  m_values[3] * other[3],
                m_values[4] * other[4],  m_values[5] * other[5],  m_values[6] * other[6],  m_values[7] * other[7],
                m_values[8] * other[8],  m_values[9] * other[9],  m_values[10] * other[10], m_values[11] * other[11],
                m_values[12] * other[12], m_values[13] * other[13], m_values[14] * other[14], m_values[15] * other[15]
            };
            return Matrix4(values);
        }

    private:
        float m_values[16] = { 0,0,0,0,
                               0,0,0,0,
                               0,0,0,0,
                               0,0,0,0 };
    };


}

float Matrices::Matrix2::operator[](int index) const
{
    return m_values[index];
}

float Matrices::Matrix3::operator[](int index) const
{
    return m_values[index];
}

float Matrices::Matrix4::operator[](int index) const
{
    return m_values[index];
}

#endif