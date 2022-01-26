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

    Vec2 Collider::circ_axis(const Circ& circ, const std::vector<Vec2>& other_vertices)
    {
        Vec2 min_vertex;
        float min_square_diff = INFINITY;

        for (auto& v : other_vertices)
        {
            const float square_diff = circ.center.distance_squared(v);
            if (square_diff < min_square_diff)
            {
                min_square_diff = square_diff;
                min_vertex = v;
            }
        }

        const Vec2 diff = circ.center - min_vertex;
        return diff.norm();
    }

    Collider::Projection Collider::projection(const std::vector<Vec2>& vertices, const Vec2& axis,
            const std::function<std::vector<Vec2> (const Vec2& vertex, const Vec2& axis)> mapper)
    {
        float min = INFINITY;
        float max = -INFINITY;

        for (auto& v : vertices)
        {
            if (mapper)
            {
                auto map_res = mapper(v, axis);
                for (auto& vm : map_res)
                {
                    const float p = vm.dot(axis);
                    min = Calc::min(p, min);
                    max = Calc::max(p, max);
                }
            }
            else
            {
                const float p = v.dot(axis);
                min = Calc::min(p, min);
                max = Calc::max(p, max);
            }
        }

        return {min, max};
    }

    bool Collider::intersects(const std::vector<Vec2>& v1, const std::vector<Vec2>& v2,
            const std::vector<Vec2>& a1, const std::vector<Vec2>& a2,
            const std::function<std::vector<Vec2> (const Vec2&, const Vec2&)> m1,
            const std::function<std::vector<Vec2> (const Vec2&, const Vec2&)> m2)
    {
        auto* vp1 = &v1;
        auto* vp2 = &v2;

        auto* mp1 = &m1;
        auto* mp2 = &m2;

        auto* ap = &a1;

        for (size_t i = 0; i < 2; i++)
        {
            if (i)
            {
                vp1 = &v2;
                vp2 = &v1;
                mp1 = &m2;
                mp2 = &m1;

                ap = &a2;
            }

            for (auto& axis : *ap)
            {
                const Projection p1 = projection(*vp1, axis, *mp1);
                const Projection p2 = projection(*vp2, axis, *mp2);

                if (!(p1.start <= p2.end && p1.end >= p2.start))
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool Collider::intersects(const Collider& other) const
    {
        auto v1 = vertices();
        auto v2 = other.vertices();

        auto a1 = axes(v2);
        auto a2 = other.axes(v1);

        auto m1 = vertex_mapper();
        auto m2 = other.vertex_mapper();

        return intersects(v1, v2, a1, a2, m1, m2);
    }

    Vec2 Collider::displace(const std::vector<Vec2>& v1, const std::vector<Vec2>& v2,
            const std::vector<Vec2>& a1, const std::vector<Vec2>& a2,
            const std::function<std::vector<Vec2> (const Vec2&, const Vec2&)> m1,
            const std::function<std::vector<Vec2> (const Vec2&, const Vec2&)> m2)
    {
        float min_overlap = INFINITY;
        Vec2 dis_axis;
        
        auto* vp1 = &v1;
        auto* vp2 = &v2;

        auto* mp1 = &m1;
        auto* mp2 = &m2;

        auto* ap = &a1;

        for (size_t i = 0; i < 2; i++)
        {
            if (i)
            {
                vp1 = &v2;
                vp2 = &v1;
                mp1 = &m2;
                mp2 = &m1;

                ap = &a2;
            }

            for (auto& axis : *ap)
            {
                const Projection p1 = projection(*vp1, axis, *mp1);
                const Projection p2 = projection(*vp2, axis, *mp2);

                if (!(p1.start <= p2.end && p1.end >= p2.start))
                {
                    return Vec2::zero;
                }

                const float overlap = Calc::min(p1.end, p2.end) - Calc::max(p1.start, p2.start);
                if (overlap < min_overlap)
                {
                    min_overlap = overlap;
                    
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

    float Collider::distance(const std::vector<Vec2>& v1, const std::vector<Vec2>& v2, 
            const std::vector<Vec2>& a1, const std::vector<Vec2>& a2, 
            const std::function<std::vector<Vec2> (const Vec2& v, const Vec2& a)> m1, 
            const std::function<std::vector<Vec2> (const Vec2& v, const Vec2& a)> m2)
    {
        float max_dist = 0.0f;
        
        auto* vp1 = &v1;
        auto* vp2 = &v2;

        auto* mp1 = &m1;
        auto* mp2 = &m2;

        auto* ap = &a1;

        for (size_t i = 0; i < 2; i++)
        {
            if (i)
            {
                vp1 = &v2;
                vp2 = &v1;
                mp1 = &m2;
                mp2 = &m1;

                ap = &a2;
            }

            for (auto& axis : *ap)
            {
                const Projection p1 = projection(*vp1, axis, *mp1);
                const Projection p2 = projection(*vp2, axis, *mp2);

                const float dist = Calc::max(p1.start, p2.start) - Calc::min(p1.end, p2.end);
                if (dist > max_dist)
                {
                    max_dist = dist;
                }
            }
        }

        return max_dist;
    }


    Vec2 Collider::displace(const Collider& other) const
    {
        auto v1 = vertices();
        auto v2 = other.vertices();

        auto a1 = axes(v2);
        auto a2 = other.axes(v1);

        auto m1 = vertex_mapper();
        auto m2 = other.vertex_mapper();

        return displace(v1, v2, a1, a2, m1, m2);
    }

    float Collider::distance(const Collider& other) const
    {
        auto v1 = vertices();
        auto v2 = other.vertices();

        auto a1 = axes(v2);
        auto a2 = other.axes(v1);

        auto m1 = vertex_mapper();
        auto m2 = other.vertex_mapper();

        return distance(v1, v2, a1, a2, m1, m2);
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
