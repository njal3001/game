#include "sb/collider.h"
#include <vector>
#include <stdio.h>
#include <cmath>
#include "engine/maths/calc.h"

namespace SB
{
    using namespace Engine;

    bool Collider::contains(const Vec2& point) const
    {
        return m_shape->contains(point);
    }

    bool Collider::intersects(const Line& line) const
    {
        return m_shape->intersects(line);
    }

    bool Collider::intersects(const Collider& other) const
    {
        const Collider* c1 = this;
        const Collider* c2 = &other;

        for (size_t i = 0; i < 2; i++)
        {
            if (i)
            {
                c1 = &other;
                c2 = this;
            }

            for (auto& axis : c1->get_axes(*c2))
            {
                Projection p1 = c1->get_projection(axis);
                Projection p2 = c2->get_projection(axis);

                if (!(p1.start <= p2.end && p1.end >= p2.start))
                {
                    return false;
                }
            }
        }

        return true;
    }

    Vec2 Collider::static_displacement(const Collider& other) const
    {
        const Collider* c1 = this;
        const Collider* c2 = &other;

        float min_overlap = INFINITY;
        Vec2 dis_axis;

        for (size_t i = 0; i < 2; i++)
        {
            if (i)
            {
                c1 = &other;
                c2 = this;
            }

            for (auto& axis : c1->get_axes(*c2))
            {
                Vec2 norm_axis = axis.norm();
                Projection p1 = c1->get_projection(norm_axis);
                Projection p2 = c2->get_projection(norm_axis);

                if (!(p1.start <= p2.end && p1.end >= p2.start))
                {
                    return Vec2();
                }

                const float overlap = Calc::min(p1.end, p2.end) - Calc::max(p1.start, p2.start);
                if (overlap < min_overlap)
                {
                    min_overlap = overlap;
                    dis_axis = norm_axis * (i ? -1.0f : 1.0f);

                    if (p1.end < p2.end)
                    {
                        dis_axis *= -1.0f;
                    }
                }
            }
        }

        return dis_axis * min_overlap;
    }
}
