#include "sb/collider.h"
#include <vector>
#include <stdio.h>
#include <cmath>
#include "engine/maths/calc.h"

namespace SB
{
    using namespace Engine;

    Collider::Collider()
        : mask(Mask::None), color(Color::red)
    {
        visible = false;
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

            for (const auto& axis : c1->axes(*c2))
            {
                const Projection p1 = c1->projection(axis);
                const Projection p2 = c2->projection(axis);

                if (!(p1.start <= p2.end && p1.end >= p2.start))
                {
                    return false;
                }
            }
        }

        return true;
    }

    Vec2 Collider::displace(const Collider& other) const
    {
        float min_overlap = INFINITY;
        Vec2 dis_axis;
        
        const Collider* c1 = this;
        const Collider* c2 = &other;

        for (size_t i = 0; i < 2; i++)
        {
            if (i)
            {
                c1 = &other;
                c2 = this;
            }

            for (const auto& axis : c1->axes(*c2))
            {
                const Projection p1 = c1->projection(axis);
                const Projection p2 = c2->projection(axis);

                if (!(p1.start <= p2.end && p1.end >= p2.start))
                {
                    return Vec2::zero;
                }

                const float overlap = Calc::min(p1.end, p2.end) - Calc::max(p1.start, p2.start);
                if (overlap < min_overlap)
                {
                    min_overlap = overlap;
                    
                    // TODO: Probably cleaner way to do this
                    // Find the push out direction
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

    float Collider::distance(const Collider& other) const
    {
        const Collider* c1 = this;
        const Collider* c2 = &other;

        float max_dist = 0.0f;

        for (size_t i = 0; i < 2; i++)
        {
            if (i)
            {
                c1 = &other;
                c2 = this;
            }

            for (const auto& axis : c1->axes(*c2))
            {
                const Projection p1 = c1->projection(axis);
                const Projection p2 = c2->projection(axis);

                const float dist = Calc::max(p1.start, p2.start) - Calc::min(p1.end, p2.end);
                if (dist > max_dist)
                {
                    max_dist = dist;
                }
            }
        }

        return max_dist;
    }

    bool Collider::check(const uint32_t mask) const
    {
        std::vector<Collider*> out;
        scene()->all(&out, mask);

        for (auto c : out)
        {
            if (c != this && c->intersects(*this))
            {
                return true;
            }
        }

        return false;
    }
}
