#include "sb/collider.h"
#include <vector>
#include <stdio.h>

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
}
