#include "sb/boxcollider.h"
#include "engine/maths/calc.h"
#include <assert.h>
#include <vector>

namespace SB
{
    using namespace Engine;

    BoxCollider::BoxCollider(const Rect& bounds)
        : bounds(bounds), rotation(0.0f)
    {}

    BoxCollider::BoxCollider(const Engine::Rect& bounds, const float rotation)
        :  bounds(bounds), rotation(rotation)
    {}

    Vec2 BoxCollider::offset() const
    {
        return Vec2(bounds.x, bounds.y);
    }

    std::vector<Vec2> BoxCollider::axes(const std::vector<Vec2>& other_vertices) const
    {
        // TODO: Could just do this with Vec2 rotations
        const Quad q(bounds, rotation);
        std::vector<Vec2> ax;

        ax.insert(ax.end(), 
        {
            (q.b - q.a).norm(),
            (q.d - q.a).norm(),
        });

        return ax;
    }

    std::vector<Vec2> BoxCollider::vertices() const
    {
        const Rect rect = bounds.offset(m_entity->pos - (Vec2(bounds.w, bounds.h) / 2.0f));
        const Quad quad(rect, rotation);

        std::vector<Vec2> vert;
        vert.insert(vert.end(), 
        {
            quad.a,
            quad.b,
            quad.c,
            quad.d
        });

        return vert;
    }

    std::function<std::vector<Vec2> (const Vec2& vertex, const Vec2& axis)>
        BoxCollider::vertex_mapper() const
    {
        return nullptr;
    }

    void BoxCollider::render(Renderer* renderer)
    {
        const Rect r = bounds.offset(m_entity->pos - (Vec2(bounds.w, bounds.h) / 2.0f));
        const Quad q(r, rotation);
        renderer->quad(q, color);
    }
}
