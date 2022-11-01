#include "quaternion.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

Quaternion Quaternion::getNormalized() const
{
    double length = sqrt(Quaternion::p_getLengthSquared());
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