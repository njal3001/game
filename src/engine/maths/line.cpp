#include "engine/maths/line.h"
#include "engine/maths/calc.h"

namespace Engine
{
    Line::Line(const Vec2& start, const Vec2& end)
        : start(start), end(end)
    {}

    float Line::len() const
    {
        return start.distance(end);
    }

    float Line::len_squared() const
    {
        return start.distance_squared(end);
    }

    float Line::distance(const Vec2& point) const
    {
        const float l2 = len_squared();
        if (l2 == 0.0f)
        {
            return point.distance(start);
        }

        const Vec2 ps = point - start;
        const Vec2 es = end - start;

        const float t = Calc::max(0.0f, Calc::min(1.0f, ps.dot(es) / l2));

        const Vec2 proj = start + (end - start) * t;
        return point.distance(proj);
    }
}
