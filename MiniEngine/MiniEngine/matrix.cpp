#include "matrix.h"

Matrix4::Matrix4()
{
    identity();
}

Matrix4::Matrix4(const float values[16])
{
    for (int i = 0; i < 16; i++)
    {
        m_values[i] = values[i];
    }
}

/*
    | 1 0 0 0 |
    | 0 1 0 0 |
    | 0 0 1 0 | 
    | 0 0 0 1 |
*/
Matrix4 Matrix4::identity()
{
    m_values[0]  = 1.0f;
    m_values[5]  = 1.0f;
    m_values[10] = 1.0f;
    m_values[15] = 1.0f;

    m_values[1] = 0.0f;
    m_values[2] = 0.0f;
    m_values[3] = 0.0f;
    m_values[4] = 0.0f;
    m_values[6] = 0.0f;
    m_values[7] = 0.0f;
    m_values[8] = 0.0f;
    m_values[9] = 0.0f;
    m_values[11] = 0.0f;
    m_values[12] = 0.0f;
    m_values[13] = 0.0f;
    m_values[14] = 0.0f;

    return *this;
}

void Matrix4::translate(int x, int y, int z)
{
    
}

void Matrix4::setRow(int index, Vector3& vector)
{
    m_values[index]     = vector.x();
    m_values[index + 4] = vector.y();
    m_values[index + 8] = vector.z();
}

void Matrix4::setColumn(int index, Vector3& vector)
{
    m_values[index * 4]     = vector.x();
    m_values[index * 4 + 1] = vector.y();
    m_values[index * 4 + 2] = vector.z();
}

Matrix4& Matrix4::lookAt(Vector3& target)
{
    Vector3 yUp = Vector3(0, 1, 0);

    // Compute the forward vector, facing the target
    Vector3 position = Vector3(m_values[12], m_values[13], m_values[14]);
    Vector3 forward  = target - position;

    // Normalize the forward vector
    forward.normalize();

    // Get the left vector
    Vector3 left = yUp.cross(forward);
    left.normalize();

    // Get the up vector
    Vector3 up = forward.cross(left);
    up.normalize();

    // Update the columns with the updated orientation
    this->setColumn(0, left);
    this->setColumn(1, up);
    this->setColumn(2, forward);

    return *this;
}
