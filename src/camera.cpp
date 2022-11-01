#include "camera.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

Camera::Camera()
{
    
}

// https://www.3dgep.com/understanding-the-view-matrix/
const Matrix4 Camera::getViewMatrix()
{
    Matrix4 m = getTransform().getMatrix();
    return m.getInverse();
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
const Matrix4 Camera::getProjectionMatrix(const double width, const double height)
{
    const double n = m_nearClip + 0.00001;
    const double f = m_farClip;

    const double fov = RADIANS(getFieldOfView());
    const double aspect = width / height;
    const double scale = tan(fov * 0.5) * n;

    const double r = aspect * scale;
    const double l = -r;
    const double t = scale;
    const double b = -t;

    const double rl = r - l;
    const double tb = t - b;
    const double nf = f - n;

    Matrix4 proj;
    proj[0][0] = 2.0 * n / rl;
    proj[1][1] = 2.0 * n / tb;

    proj[0][2] =  (r + l) / rl;
    proj[1][2] =  (t + b) / tb;
    proj[2][2] = -(f + n) / nf;
    proj[3][2] = -1.0;               // Left or right handed

    proj[2][3] = -2.0 * n * f / nf;
    proj[3][3] =  0.0;

    return proj;
}

MINI_NAMESPACE_CLOSE