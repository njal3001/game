#include "sb/collider.h"
#include <vector>
#include <stdio.h>
#include <cmath>
#include "engine/maths/calc.h"

namespace SB
{
    using namespace Engine;

    bool Collider::intersects(const Vec2& pos, const Vec2& pos_other, const Collider& other) const
    {
        // TODO: Messy..
        const Collider* c1 = this;
        const Vec2* pos1 = &pos;
        const Collider* c2 = &other;
        const Vec2* pos2 = &pos_other;

        for (size_t i = 0; i < 2; i++)
        {
            if (i)
            {
                c1 = &other;
                pos1 = &pos_other;
                c2 = this;
                pos2 = &pos;
            }

            for (auto& axis : c1->get_axes(*pos1, *pos2))
            {
                Projection p1 = c1->get_projection(*pos1, axis);
                Projection p2 = c2->get_projection(*pos2, axis);

                if (!(p1.start <= p2.end && p1.end >= p2.start))
                {
                    return false;
                }
            }
        }

        return true;
    }

    Vec2 Collider::static_displacement(const Vec2& pos, const Vec2& pos_other, const Collider& other) const
    {
        const Collider* c1 = this;
        const Vec2* pos1 = &pos;
        const Collider* c2 = &other;
        const Vec2* pos2 = &pos_other;

        float min_overlap = INFINITY;
        Vec2 dis_axis;

        for (size_t i = 0; i < 2; i++)
        {
            if (i)
            {
                c1 = &other;
                pos1 = &pos_other;
                c2 = this;
                pos2 = &pos;
            }

            for (auto& axis : c1->get_axes(*pos1, *pos2))
            {
                Projection p1 = c1->get_projection(*pos1, axis);
                Projection p2 = c2->get_projection(*pos2, axis);

                if (!(p1.start <= p2.end && p1.end >= p2.start))
                {
                    return Vec2();
                }

                const float overlap = Calc::min(p1.end, p2.end) - Calc::max(p1.start, p2.start);
                if (overlap < min_overlap)
                {
                    min_overlap = overlap;
                    dis_axis = axis * (i ? -1.0f : 1.0f);

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
