#include "quaternion.h"

Quaternion Quaternion::getNormalized() const
{
    double length = sqrt(Quaternion::p_getLengthSquared());
    if (length < Math::EPSILON)
    {
        return Quaternion::getIdentity();
    }
    else
    {
        return (*this); // / length;
    }
}