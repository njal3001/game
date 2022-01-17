#include "sb/circlecollider.h"
#include "engine/maths/calc.h"

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

    /* bool CircleCollider::contains(const Engine::Vec2& point) const */
    /* { */
    /*     const Circ circ(m_entity->pos + bounds.center, bounds.radius); */
    /*     return circ.contains(point); */
    /* } */

    /* bool CircleCollider::intersects(const Engine::Line& line) const */
    /* { */
    /*     const Circ circ(m_entity->pos + bounds.center, bounds.radius); */
    /*     return circ.intersects(line); */
    /* } */

    std::vector<Vec2> CircleCollider::axes(const std::vector<Vec2>& other_vertices) const
    {
        std::vector<Vec2> axes;
        axes.push_back(circ_axis(bounds.offset(m_entity->pos), other_vertices));

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

    void CircleCollider::render(Engine::Renderer* renderer)
    {
        renderer->circ(bounds.offset(m_entity->pos), 128, color);
    }
}
