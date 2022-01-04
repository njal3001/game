#include "engine/maths/circ.h"

namespace Engine
{
    Circ::Circ()
        : Circ(Vec2::zero, 0.0f)
    {}

    Circ::Circ(const Vec2& center, const float radius)
        : center(center), radius(radius)
    {}

    bool Circ::contains(const Vec2& point) const
    {
        return point.distance_squared(center) <= radius * radius;
    }

    bool Circ::intersects(const Line& line) const
    {
        return line.distance(center) <= radius;
    }

    bool Circ::intersects(const Circ& other) const
    {
        const float r_sum = radius + other.radius;
        return center.distance_squared(other.center) <= r_sum * r_sum;
    }
}
