#include "engine/maths/circ.h"

namespace Engine
{
    Circ::Circ()
        : radius(0.0f)
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
}
