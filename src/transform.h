#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"

namespace Graphics
{
    class Transform
    {
     public:
        Transform() = default;;

        Transform& identity();

        [[nodiscard]] Matrix4 getMatrix() const;
        void setMatrix(Matrix4& m)
        {
            m_translation = m.getTranslation();
            m_rotation = m.getRotation();
        }

        void setTranslation(const Vector3& t);
        const Vector3& getTranslation()
        {
            return m_translation;
        }
        void setRotation(const Rotation& r);
        const Rotation& getRotation()
        {
            return m_rotation;
        }
        void setScale(const Vector3& s);
        const Vector3& getScale()
        {
            return m_scale;
        }

        Vector3 getForward();
        Vector3 getRight();
        Vector3 getUp();

     private:
        Vector3 m_translation;
        Rotation m_rotation;
        Vector3 m_scale;
    };

}

#endif