#include "sb/collider.h"
#include <vector>
#include <stdio.h>
#include <cmath>
#include "engine/maths/calc.h"

namespace SB
{
    using namespace Engine;

    const std::vector<Vec2> Collider::rect_axes =
    { Vec2(1.0f, 0.0f), Vec2(0.0f, 1.0f) };

    Vec2 Collider::circ_axis(const Circ& circ, const Collider& other)
    {
        const Vec2 diff = circ.center - (other.entity()->pos + other.offset());
        return diff.norm();
    }

    std::vector<Vec2> Collider::rect_vertices(const Rect& rect)
    {
        const std::vector<Vec2> vert = 
        {
            rect.top_left(),
            rect.bottom_left(),
            rect.top_right(),
            rect.bottom_right()
        };

        return vert;
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
                const Projection p1 = projection(v1, axis, *mp1);
                const Projection p2 = projection(v2, axis, *mp2);

                printf("p1: (%f, %f)\n", p1.start, p1.end);
                printf("p2: (%f, %f)\n", p2.start, p2.end);
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

        auto a1 = axes(other);
        auto a2 = other.axes(*this);

        auto m1 = vertex_mapper();
        auto m2 = other.vertex_mapper();

        /* for (auto v : v1) */
        /*     printf("v1: (%f, %f)\n", v.x, v.y); */

        /* for (auto v : v2) */
        /*     printf("v2: (%f, %f)\n", v.x, v.y); */

        /* for (auto a : a1) */
        /*     printf("a: (%f, %f)\n", a.x, a.y); */

        /* for (auto a : a2) */
        /*     printf("a2: (%f, %f)\n", a.x, a.y); */

        return intersects(v1, v2, a1, a2, m1, m2);
        /* const Collider* c1 = this; */
        /* const Collider* c2 = &other; */

        /* for (size_t i = 0; i < 2; i++) */
        /* { */
        /*     if (i) */
        /*     { */
        /*         c1 = &other; */
        /*         c2 = this; */
        /*     } */

        /*     for (auto& axis : c1->axes(*c2)) */
        /*     { */
        /*         const auto v1 = c1->vertices(axis); */
        /*         const auto v2 = c2->vertices(axis); */
        /*         const Projection p1 = projection(v1, axis); */
        /*         const Projection p2 = projection(v2, axis); */

        /*         if (!(p1.start <= p2.end && p1.end >= p2.start)) */
        /*         { */
        /*             return false; */
        /*         } */
        /*     } */
        /* } */

        /* return true; */
    }

    bool Collider::intersects(const Engine::Rect& rect) const
    {
        /* for (auto */ 
        /* return intersects(vertices(), */ 
        return false;
    }

    bool Collider::intersects(const Engine::Circ& circ) const
    {
        return false;
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
                const Projection p1 = projection(v1, axis, *mp1);
                const Projection p2 = projection(v2, axis, *mp2);

                if (!(p1.start <= p2.end && p1.end >= p2.start))
                {
                    return Vec2::zero;
                }

                const float overlap = Calc::min(p1.end, p2.end) - Calc::max(p1.start, p2.start);
                if (overlap < min_overlap)
                {
                    min_overlap = overlap;
                    dis_axis = axis * (i ? -1.0f : 1.0f);
                }
            }
        }

        return dis_axis * min_overlap;
    }


    Vec2 Collider::displace(const Collider& other) const
    {
        auto v1 = vertices();
        auto v2 = other.vertices();

        printf("v1: (%f, %f)\n", v1[0].x, v1[0].y); 
        printf("v2: (%f, %f)\n", v2[0].x, v2[0].y); 
        
        auto a1 = axes(other);
        auto a2 = other.axes(*this);

        printf("a1: (%f, %f)\n", a1[0].x, a1[0].y); 
        printf("a2: (%f, %f)\n", a2[0].x, a2[0].y); 

        auto m1 = vertex_mapper();
        auto m2 = other.vertex_mapper();

        return displace(v1, v2, a1, a2, m1, m2);
        /* const Collider* c1 = this; */
        /* const Collider* c2 = &other; */

        /* float min_overlap = INFINITY; */
        /* Vec2 dis_axis; */

        /* for (size_t i = 0; i < 2; i++) */
        /* { */
        /*     if (i) */
        /*     { */
        /*         c1 = &other; */
        /*         c2 = this; */
        /*     } */

        /*     for (auto& axis : c1->axes(*c2)) */
        /*     { */
        /*         const auto v1 = c1->vertices(); */
        /*         const auto v2 = c2->vertices(); */
        /*         const Projection p1 = projection(v1, axis); */
        /*         const Projection p2 = projection(v2, axis); */

        /*         if (!(p1.start <= p2.end && p1.end >= p2.start)) */
        /*         { */
        /*             return Vec2::zero; */
        /*         } */

        /*         const float overlap = Calc::min(p1.end, p2.end) - Calc::max(p1.start, p2.start); */
        /*         if (overlap < min_overlap) */
        /*         { */
        /*             min_overlap = overlap; */
        /*             dis_axis = axis * (i ? -1.0f : 1.0f); */

        /*             if (p1.end < p2.end) */
        /*             { */
        /*                 dis_axis *= -1.0f; */
        /*             } */
        /*         } */
        /*     } */
        /* } */

        /* return dis_axis * min_overlap; */
    }
}
