#ifndef MATRIX_H
#define MATRIX_H
#endif

#include "vector.h"

const static float EPSILON = 0.00001f;

class Matrix4
{
public:
    Matrix4();
    Matrix4(const float values[16]);

    Matrix4 identity();
    void translate(int x, int y, int z);

    Matrix4& lookAt(Vector3& target);

    void setRow(int index, Vector3& vector);
    void setColumn(int index, Vector3& vector);

    float operator[](int index) const;

private:
    float m_values[16] = {0,0,0,0,
                          0,0,0,0,
                          0,0,0,0,
                          0,0,0,0};
};

float Matrix4::operator[](int index) const
{
    return m_values[index];
}