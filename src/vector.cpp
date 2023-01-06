#include "vector.h"

namespace Graphics
{
    std::string Vector2::toString() const
    {
        std::string string("[");
        string += std::to_string(_x);
        string += ",";
        string += std::to_string(_y);
        string += "]";
        return string;
    }
    Vector2 Vector2::operator+(Vector2& v) const
    {
        return { _x + v._x, _y + v._y };
    }
    Vector2 Vector2::operator-(Vector2& v) const
    {
        return { _x - v._x, _y - v._y };
    }
    Vector2 Vector2::operator*(Vector2& v) const
    {
        return { _x * v._x, _y * v._y };
    }
    Vector2 Vector2::operator/(Vector2& v) const
    {
        return { _x / v._x, _y / v._y };
    }
    bool Vector2::operator<(Vector2& v) const
    {
        return _x < v._x && _y < v._y;
    }
    bool Vector2::operator>(Vector2& v) const
    {
        return _x > v._x && _y > v._y;
    }
    bool Vector2::operator==(Vector2& v) const
    {
        return _x - v._x < EPSILON && _y - v._y < EPSILON;
    }

    Vector3::Vector3()
    {
        _x = 0;
        _y = 0;
        _z = 0;
    };

    Vector3::Vector3(double x, double y, double z)
    {
        _x = x;
        _y = y;
        _z = z;
    };

    Vector3::Vector3(Vector2& v)
    {
        _x = v._x;
        _y = v._y;
        _z = 0.0;
    };

    Vector3::Vector3(Vector2& v, double z)
    {
        _x = v._x;
        _y = v._y;
        _z = z;
    };

    void Vector3::normalize()
    {
        double l = length();
        _x /= l;
        _y /= l;
        _z /= l;
    }

    Vector3 Vector3::identity()
    {
        return { 1.0, 1.0, 1.0 };
    }

    Vector3::Vector3(double v)
    {
        *this = Vector3(v, v, v);
    }

    std::string Vector3::toString() const
    {
        std::string string("[");
        string += std::to_string(_x);
        string += ",";
        string += std::to_string(_y);
        string += ",";
        string += std::to_string(_z);
        string += "]";
        return string;
    }

    double Vector3::length() const
    {
        return sqrt(_x * _x + _y * _y + _z * _z);
    }

    double Vector3::operator[](int i) const
    {
        if (i == 0)
        {
            return _x;
        }
        else if (i == 1)
        {
            return _y;
        }
        else if (i == 2)
        {
            return _z;
        }

        return 0;
    }

    void Vector3::set(double x, double y, double z)
    {
        _x = x, _y = y, _z = z;
    }
    bool Vector3::operator==(const Vector3& v) const
    {
        return (_x == v._x &&
            _y == v._y &&
            _z == v._z);
    }
    Vector3 Vector3::operator+(Vector3& v) const
    {
        return {_x + v._x, _y + v._y, _z + v._z};
    }
    Vector3 Vector3::operator+(const Vector3& v) const
    {
        return {_x + v._x, _y + v._y, _z + v._z};
    }
    Vector3 Vector3::operator+=(Vector3& v) const
    {
        return {_x + v._x, _y + v._y, _z + v._z};
    }
    Vector3 Vector3::operator+=(const Vector3& v) const
    {
        return {_x + v._x, _y + v._y, _z + v._z};
    }
    Vector3 Vector3::operator-(Vector3& v) const
    {
        return {_x - v._x, _y - v._y, _z - v._z};
    }
    Vector3 Vector3::operator-(const Vector3& v) const
    {
        return {_x - v._x, _y - v._y, _z - v._z};
    }
    Vector3 Vector3::operator*(const Vector3& v) const
    {
        return {_x * v._x, _y * v._y, _z * v._z};
    }
    Vector3 Vector3::operator*(Vector3& v) const
    {
        return {_x * v._x, _y * v._y, _z * v._z};
    }
    Vector3 Vector3::operator*(double d) const
    {
        return {_x * d, _y * d, _z * d};
    }
    Vector3 Vector3::operator/(Vector3& v) const
    {
        return {_x / v._x, _y / v._y, _z / v._z};
    }
    Vector3 Vector3::operator/(double d) const
    {
        return {_x / d, _y / d, _z / d};
    }
    bool Vector3::operator<(double d) const
    {
        return _x < d && _y < d && _z < d;
    }
    bool Vector3::operator<(Vector3& v) const
    {
        return _x < v._x && _y < v._y && _z < v._z;
    }
    bool Vector3::operator>(Vector3& v) const
    {
        return _x > v._x && _y > v._y && _z > v._z;
    }
    bool Vector3::operator==(Vector3& v) const
    {
        return _x - v._x < EPSILON && _y - v._y < EPSILON&& _z - v._z < EPSILON;
    }
    Vector3 Vector3::operator-() const
    {
        return {-_x, -_y, -_z};
    }
    Vector3 Vector3::operator*=(Vector3& v) const
    {
        return (*this * v);
    }
    Vector3 Vector3::operator*=(double d) const
    {
        return (*this * d);
    }

    Vector4::Vector4()
    {
        _x = 0;
        _y = 0;
        _z = 0;
        _w = 0;
    }

    Vector4::Vector4(double d)
    {
        _x = d;
        _y = d;
        _z = d;
        _w = d;
    }

    Vector4::Vector4(double x, double y, double z, double w)
    {
        _x = x;
        _y = y;
        _z = z;
        _w = w;
    }

    Vector4::Vector4(const Vector3& v, double w)
    {
        _x = v._x;
        _y = v._y;
        _z = v._z;
        _w = w;
    }

    void Vector4::normalize()
    {
        double l = length();
        _x /= l;
        _y /= l;
        _z /= l;
        _w /= l;
    }

    Vector4 Vector4::identity()
    {
        return Vector4(1.0, 1.0, 1.0, 1.0);
    }
    std::string Vector4::toString() const
    {
        std::string string("[");
        string += std::to_string(_x);
        string += ",";
        string += std::to_string(_y);
        string += ",";
        string += std::to_string(_z);
        string += ",";
        string += std::to_string(_w);
        string += "]";
        return string;
    }
    Vector4 Vector4::operator+(Vector4& v) const
    {
        return {_x + v._x, _y + v._y, _z + v._z, _w + v._w};
    }
    Vector4 Vector4::operator-(Vector4& v) const
    {
        return {_x - v._x, _y - v._y, _z - v._z, _w - v._w};
    }
    Vector4 Vector4::operator*(Vector4& v) const
    {
        return {_x * v._x, _y * v._y, _z * v._z, _w * v._w};
    }
    Vector4 Vector4::operator/(Vector4& v) const
    {
        return {_x / v._x, _y / v._y, _z / v._z, _w / v._w};
    }
    bool Vector4::operator<(Vector4& v) const
    {
        return _x < v._x&& _y < v._y&& _z < v._z&& _w < v._w;
    }
    bool Vector4::operator>(Vector4& v) const
    {
        return _x > v._x && _y > v._y && _z > v._z && _w > v._w;
    }
    bool Vector4::operator==(Vector4& v) const
    {
        return _x - v._x < EPSILON && _y - v._y < EPSILON && _z - v._z < EPSILON && _w - v._w < EPSILON;
    }
    Vector4 Vector4::operator*=(Vector4& v) const
    {
        return (*this * v);
    }
}