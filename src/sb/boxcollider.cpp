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

    Vec2 BoxCollider::pos() const
    {
        return m_entity->pos + Vec2(bounds.x, bounds.y) - (Vec2(bounds.w, bounds.h) / 2.0f);
    }

    std::vector<Vec2> BoxCollider::axes(const Collider& other) const
    {
        const Quad q(bounds, rotation);
        return
        {
            (q.b - q.a).norm(),
            (q.d - q.a).norm(),
        };
    }

    Quad BoxCollider::make_quad() const
    {
        const Rect rect = bounds.offset(m_entity->pos - (Vec2(bounds.w, bounds.h) / 2.0f));
        return Quad(rect, rotation);
    }

    Collider::Projection BoxCollider::projection(const Vec2& axis) const
    {
        const Quad quad = make_quad();

        const float p0 = quad.a.dot(axis);
        const float p1 = quad.b.dot(axis);
        const float p2 = quad.c.dot(axis);
        const float p3 = quad.d.dot(axis);

        float min = p0;

        min = Calc::min(min, p1);
        min = Calc::min(min, p2);
        min = Calc::min(min, p3);

        float max = p0;

        max = Calc::max(max, p1);
        max = Calc::max(max, p2);
        max = Calc::max(max, p3);

        return {min, max};
    }

    Vec2 BoxCollider::nearest_vertex(const Vec2& pos) const
    {
        const Quad quad = make_quad();

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

    Rect BoxCollider::bounding_box() const
    {
        // TODO: Slow...
        const Quad quad = make_quad();

        float min_x = quad.a.x;
        min_x = Calc::min(min_x, quad.b.x);
        min_x = Calc::min(min_x, quad.c.x);
        min_x = Calc::min(min_x, quad.d.x);

        float max_x = quad.a.x;
        max_x = Calc::max(max_x, quad.b.x);
        max_x = Calc::max(max_x, quad.c.x);
        max_x = Calc::max(max_x, quad.d.x);

        float min_y = quad.a.y;
        min_y = Calc::min(min_y, quad.b.y);
        min_y = Calc::min(min_y, quad.c.y);
        min_y = Calc::min(min_y, quad.d.y);

        float max_y = quad.a.y;
        max_y = Calc::max(max_y, quad.b.y);
        max_y = Calc::max(max_y, quad.c.y);
        max_y = Calc::max(max_y, quad.d.y);

        return Rect(min_x, min_y, max_x - min_x, max_y - min_y);
    }

    void BoxCollider::render(Renderer* renderer)
    {
        const Rect r = bounds.offset(m_entity->pos - (Vec2(bounds.w, bounds.h) / 2.0f));
        const Quad q(r, rotation);
        renderer->quad(q, color);
    }
}
