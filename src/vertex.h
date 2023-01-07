#ifndef VERTEX_H
#define VERTEX_H

#include "vector.h"

namespace Graphics
{
    using namespace Graphics;

    class Vertex
    {
     public:
        Vertex() : m_translation(Vector3())
        {
        };
        Vertex(double x, double y, double z) : m_translation(Vector3(x, y, z))
        {
        };
        explicit Vertex(Vector3* v) : m_translation(*v)
        {
        };
        explicit Vertex(Vector3& v) : m_translation(v)
        {
        };
        explicit Vertex(const Vector3& v) : m_translation(v)
        {
        };

        Vector3 getTranslation()
        {
            return m_translation;
        }
        void setTranslation(const Vector3& t)
        {
            m_translation = t;
        }

        Vector3 getNormal()
        {
            return m_normal;
        }
        void setNormal(const Vector3& n)
        {
            m_normal = n;
        }

        static const int stride = 32;
        static const int posOffset = 0;
        static const int colorOffset = 12;
        static const int uvOffset = 24;

     private:
        Vector3 m_translation;
        Vector3 m_normal;
    };

}

#endif