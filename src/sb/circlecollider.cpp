#include "sb/circlecollider.h"
#include "engine/maths/calc.h"
#include <assert.h>

namespace SB
{
    using namespace Engine;

    CircleCollider::CircleCollider(const Engine::Circ& bounds)
        : bounds(bounds)
    {}

    Engine::Vec2 CircleCollider::offset() const
    {
        return bounds.center;
    }

    bool CircleCollider::contains(const Engine::Vec2& point) const
    {
        const Circ circ(m_entity->pos + bounds.center, bounds.radius);
        return circ.contains(point);
    }

    bool CircleCollider::intersects(const Engine::Line& line) const
    {
        const Circ circ(m_entity->pos + bounds.center, bounds.radius);
        return circ.intersects(line);
    }

    std::vector<Vec2> CircleCollider::axes(const Collider& other) const
    {
        std::vector<Vec2> axes;
        axes.push_back(circ_axis(bounds.offset(m_entity->pos), other));

        return axes;
    }

    std::vector<Vec2> CircleCollider::vertices() const
    {
        std::vector<Vec2> v = { m_entity->pos + bounds.center };
        return v;
    }

    std::function<std::vector<Vec2> (const Vec2& vertex, const Vec2& axis)>
        CircleCollider::vertex_mapper() const
    {
        return [this](const Vec2& vertex, const Vec2& axis)
        {
            std::vector<Vec2> mapped = 
            {
                vertex + (axis * this->bounds.radius),
                vertex - (axis * this->bounds.radius), 
            };

            return mapped;
        };
    }

    /* Collider::Projection CircleCollider::projection(const Vec2& axis) const */
    /* { */
    /*     const Vec2 front = bounds.center + m_entity->pos + (axis * bounds.radius); */
    /*     const Vec2 back = bounds.center + m_entity->pos - (axis * bounds.radius); */

    /*     const float p0 = front.dot(axis); */
    /*     const float p1 = back.dot(axis); */

    /*     // TODO: Could do with one if else instead */
    /*     return {Calc::min(p0, p1), Calc::max(p0, p1)}; */
    /* } */
}
