#include "quaternion.h"

namespace Graphics {
using namespace Graphics;

Quaternion Quaternion::getNormalized() const
{
    double lengthSquared = _getLengthSquared();
    double length = sqrt(lengthSquared);
    if (length < EPSILON)
    {
        return Quaternion::getIdentity();
    }
    else
    {
        return (*this / length);
    }
}

}