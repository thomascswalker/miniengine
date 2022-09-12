#include "camera.h"

Camera::Camera()
{
    
}

/*
    https://www.scratchapixel.com/code.php?id=4&origin=/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix
    https://jsantell.com/3d-projection/
    http://xdpixel.com/decoding-a-projection-matrix/

    [ 2n / r-1 ] [    0     ] [ r+l / r-l ] [     0     ]
    [    0     ] [ 2n / t-b ] [ t+b / t-b ] [     0     ]
    [    0     ] [    0     ] [ f+n / n-f ] [ 2fn / n-f ]
    [    0     ] [    0     ] [    -1     ] [     0     ]

*/
const Matrix4 Camera::getProjectionMatrix(const float width, const float height)
{
    Matrix4 matrix;
    matrix.setIdentity();

    const double l = 0;
    const double r = width;
    const double b = height;
    const double t = 0;
    const double n = m_nearClip;
    const double f = m_farClip;

    const double rl = r - l;
    const double tb = t - b;
    const double fn = f - n;

    matrix[0][0] = 2.0 * n / r - l;
    matrix[1][1] = 2.0 * n / t - b;
    matrix[2][2] = -(f + n) / n - f;
    matrix[2][0] =  (r + l) / r - l;
    matrix[2][1] =  (t + b) / t - b;
    matrix[3][2] = -2.0 * n * f / n - f;
    matrix[2][3] = -1.0;
    matrix[3][3] =  0.0;

    return matrix;
}
