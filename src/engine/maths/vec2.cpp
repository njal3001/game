#include "engine/maths/vec2.h"
#include "engine/maths/calc.h"

namespace Engine
{
    const Vec2 Vec2::zero = Vec2();

    Vec2::Vec2()
            : x(0), y(0) {}

    Vec2::Vec2(float x, float y)
            : x(x), y(y) {}

    Vec2::Vec2(int x, int y)
            : x((float)x), y((float)y) {}


    Vec2 Vec2::operator -() const
    {
        return Vec2(-x, -y);
    }

    Vec2 Vec2::operator +(const Vec2& rhs) const
    {
        return Vec2(x + rhs.x, y + rhs.y);
    }

    Vec2 Vec2::operator -(const Vec2& rhs) const
    {
        return Vec2(x - rhs.x, y - rhs.y);
    }

    Vec2 Vec2::operator *(const float rhs) const
    {
        return Vec2(x * rhs, y * rhs);
    }

    Vec2 Vec2::operator /(const float rhs) const
    {
        return Vec2(x / rhs, y / rhs);
    }


    Vec2& Vec2::operator +=(const Vec2& rhs)
    {
        x += rhs.x;
        y += rhs.y;

        return *this;
    }

    Vec2& Vec2::operator -=(const Vec2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;

        return *this;
    }

    Vec2& Vec2::operator *=(const float rhs)
    {
        x *= rhs;
        y *= rhs;

        return *this;
    }

    Vec2& Vec2::operator /=(const float rhs)
    {
        x /= rhs;
        y /= rhs;

        return *this;
    }

    bool Vec2::operator ==(const Vec2& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    bool Vec2::operator !=(const Vec2& rhs) const
    {
        return x != rhs.x || y != rhs.y;
    }

    float Vec2::len() const
    {
        return Calc::sqrt(x * x + y * y);
    }


    float Vec2::len_squared() const
    {
        return x * x + y* y;
    }

    Vec2 Vec2::norm() const
    {
        if (x == 0 && y == 0)
        {
            return Vec2::zero;
        }

        const float l = len();

        return Vec2(x / l, y / l);
    }

    float Vec2::distance(const Vec2& other) const
    {
        return Calc::sqrt(distance_squared(other));
    }

    float Vec2::distance_squared(const Vec2& other) const
    {
        float dx = x - other.x;
        float dy = y - other.y;
        return dx * dx + dy * dy;
    }

    float Vec2::dot(const Vec2& other) const
    {
        return x * other.x + y * other.y;
    }
    
    Vec2 Vec2::approach(const Vec2& val, const Vec2& target, const float amount)
    {
        if (val.distance_squared(target) <= amount)
        {
            return target;
        }

        Vec2 dir = (target - val).norm();
        return val + dir * amount;
    }

    Vec2 Vec2::lerp(const Vec2& a, const Vec2& b, const float t)
    {
        return a + (b - a) * t;
    }
}
