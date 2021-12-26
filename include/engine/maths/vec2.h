#pragma once

namespace Engine
{
    struct Vec2
    {
        float x, y;

        Vec2();

        Vec2(float x, float y);

        Vec2(int x, int y);

        Vec2 operator -() const;

        Vec2 operator +(const Vec2& rhs) const;
        Vec2 operator -(const Vec2& rhs) const;
        Vec2 operator *(const float rhs) const;
        Vec2 operator /(const float rhs) const;

        Vec2& operator +=(const Vec2& rhs);
        Vec2& operator -=(const Vec2& rhs);
        Vec2& operator *=(const float rhs);
        Vec2& operator /=(const float rhs);

        bool operator ==(const Vec2& rhs) const;
        bool operator !=(const Vec2& rhs) const;

        float len() const;
        float len_squared() const;
        Vec2 norm() const;
        float distance(const Vec2& other) const;
        float distance_squared(const Vec2& other) const;
        float dot(const Vec2& other) const;

        static const Vec2 zero;
    };
}
