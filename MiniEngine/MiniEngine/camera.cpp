#include "camera.h"

Camera::Camera()
{
    
}

const Matrix4 Camera::getViewMatrix()
{
    auto t = getTransform();
    Vector3 eye = t.getTranslation();
    double pitch = 0;
    double yaw = 0;

    double cp = cos(pitch);
    double sp = sin(pitch);
    double cy = cos(yaw);
    double sy = sin(yaw);

    Vector3 x(cy, 0, -sy);
    Vector3 y(sy * sp, cp, cy * sp);
    Vector3 z(sy * cp, -sp, cp * cy);

    Matrix4 view;

    view[0][0] = x.x();
    view[0][1] = y.x();
    view[0][2] = z.x();
    view[0][3] = 0;

    view[1][0] = x.y();
    view[1][1] = y.y();
    view[1][2] = z.y();
    view[1][3] = 0;

    view[2][0] = x.z();
    view[2][1] = y.z();
    view[2][2] = z.z();
    view[2][3] = 0;

    view[3][0] = Math::dot(x, eye) * -1;
    view[3][1] = Math::dot(y, eye) * -1;
    view[3][2] = Math::dot(z, eye) * -1;
    view[3][3] = 1;
    
    return view;
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
    Matrix4 proj;
    proj.setIdentity();

    double aspect = width / height;
    double scale = tan(getFieldOfView() * 0.5 * Math::PI / 180.0) * m_nearClip;

    const double r = aspect * scale;
    const double l = -r;
    const double t = scale;
    const double b = -t;
    const double n = m_nearClip;
    const double f = m_farClip;

    const double rl = r - l;
    const double tb = t - b;
    const double nf = f - n;

    proj[0][0] = 2.0 * n / rl;
    proj[1][1] = 2.0 * n / tb;
    proj[2][2] = -(f + n) / nf;
    proj[2][0] =  (r + l) / rl;
    proj[2][1] =  (t + b) / tb;
    proj[3][2] = -2.0 * n * f / nf;
    proj[2][3] = -1.0;
    proj[3][3] =  0.0;

    return proj;
}
