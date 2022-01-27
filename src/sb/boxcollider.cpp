#include "sb/boxcollider.h"
#include "engine/maths/calc.h"
#include <assert.h>
#include <vector>
#include <cmath>

namespace SB
{
    using namespace Engine;

    BoxCollider::BoxCollider(const Rect& bounds)
        : bounds(bounds), rotation(0.0f)
    {}

    BoxCollider::BoxCollider(const Rect& bounds, const float rotation)
        : bounds(bounds), rotation(rotation)
    {}

    std::vector<Vec2> BoxCollider::axes(const Collider& other) const
    {
        const Quad q(bounds, rotation);
        return 
        {
            (q.b - q.a).norm(),
            (q.d - q.a).norm(),
        };
    }

    Collider::Projection BoxCollider::projection(const Vec2& axis) const
    {
        const Rect rect = bounds.offset(m_entity->pos - (Vec2(bounds.w, bounds.h) / 2.0f));
        const Quad quad(rect, rotation);

        const float p0 = quad.a.dot(axis);
        const float p1 = quad.b.dot(axis);
        const float p2 = quad.c.dot(axis);
        const float p3 = quad.d.dot(axis);

        float min = p0;

        min = Calc::min(min, p1);
        min = Calc::min(min, p2);
        min = Calc::min(min, p3);

        float max = p1;

        max = Calc::max(max, p1);
        max = Calc::max(max, p2);
        max = Calc::max(max, p3);

        return {min, max};
    }

    Vec2 BoxCollider::nearest_vertex(const Vec2& pos) const
    {
        const Rect rect = bounds.offset(m_entity->pos - (Vec2(bounds.w, bounds.h) / 2.0f));
        const Quad quad(rect, rotation);

        Vec2 min_vertex;
        float min_square_diff = INFINITY;

        const Vec2 vertices[4] = {quad.a, quad.b, quad.c, quad.d};
        for (const auto& v : vertices)
        {
            const float square_diff = pos.distance_squared(v);
            if (square_diff < min_square_diff)
            {
                min_square_diff = square_diff;
                min_vertex = v;
            }
        }

        return min_vertex;
    }

    void BoxCollider::render(Renderer* renderer)
    {
        const Rect r = bounds.offset(m_entity->pos - (Vec2(bounds.w, bounds.h) / 2.0f));
        const Quad q(r, rotation);
        renderer->quad(q, color);
    }
}
