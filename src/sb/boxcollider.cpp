#include "sb/boxcollider.h"
#include "engine/maths/calc.h"
#include <assert.h>
#include <vector>

namespace SB
{
    using namespace Engine;

    BoxCollider::BoxCollider(const Rect& bounds)
        : bounds(bounds)
    {}

    Vec2 BoxCollider::offset() const
    {
        return Vec2(bounds.x, bounds.y);
    }

    bool BoxCollider::contains(const Vec2& point) const
    {
        const Rect rect = bounds.offset(m_entity->pos - (Vec2(bounds.w, bounds.h) / 2.0f));
        return rect.contains(point);
    }

    bool BoxCollider::intersects(const Line& line) const
    {
        const Rect rect = bounds.offset(m_entity->pos - (Vec2(bounds.w, bounds.h) / 2.0f));
        return rect.intersects(line);
    }
    
    std::vector<Vec2> BoxCollider::axes(const std::vector<Vec2>& other_vertices) const
    {
        return Collider::rect_axes;
    }

    std::vector<Vec2> BoxCollider::vertices() const
    {
        const Rect rect = bounds.offset(m_entity->pos - (Vec2(bounds.w, bounds.h) / 2.0f));
        return Collider::rect_vertices(rect);
    }

    std::function<std::vector<Vec2> (const Vec2& vertex, const Vec2& axis)>
        BoxCollider::vertex_mapper() const
    {
        return nullptr;
    }
}
