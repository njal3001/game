#include "maths/vec2.h"

namespace Engine
{
    Vec2::Vec2()
            : x(0), y(0) {}

    Vec2::Vec2(float x, float y)
            : x(x), y(y) {}

    Vec2::Vec2(int x, int y)
            : x((float)x), y((float)y) {}

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
}
