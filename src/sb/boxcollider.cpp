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
    
    std::vector<Vec2> BoxCollider::axes(const Collider& other) const
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

    /* Collider::Projection BoxCollider::projection(const Vec2& axis) const */
    /* { */
    /*     Rect rect = bounds.offset(m_entity->pos - (Vec2(bounds.w, bounds.h) / 2.0f)); */

    /*     const float p0 = rect.top_left().dot(axis); */
    /*     const float p1 = rect.bottom_left().dot(axis); */
    /*     const float p2 = rect.top_right().dot(axis); */
    /*     const float p3 = rect.bottom_right().dot(axis); */

    /*     float min = p0; */

    /*     min = Calc::min(min, p1); */
    /*     min = Calc::min(min, p2); */
    /*     min = Calc::min(min, p3); */

    /*     float max = p0; */

    /*     max = Calc::max(max, p1); */
    /*     max = Calc::max(max, p2); */
    /*     max = Calc::max(max, p3); */

    /*     return {min, max}; */
    /* } */
}
