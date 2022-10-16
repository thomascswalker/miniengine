#include "quaternion.h"

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
