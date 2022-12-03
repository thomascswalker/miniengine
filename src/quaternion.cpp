#include "quaternion.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

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

MINI_NAMESPACE_CLOSE