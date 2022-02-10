#include "sb/circlecollider.h"
#include "engine/maths/calc.h"

namespace SB
{
    using namespace Engine;

    CircleCollider::CircleCollider(const Engine::Circ& bounds)
        : bounds(bounds)
    {}

    Vec2 CircleCollider::pos() const
    {
        return m_entity->pos + bounds.center;
    }

    std::vector<Engine::Vec2> CircleCollider::axes(const Collider& other) const
    {
        const Vec2 pos = m_entity->pos + bounds.center;
        const Vec2 diff = pos - other.nearest_vertex(pos);
        return { diff.norm() };
    }

    Collider::Projection CircleCollider::projection(const Engine::Vec2& axis) const
    {
        const Vec2 pos = m_entity->pos + bounds.center;
        const Vec2 front = pos + (axis * bounds.radius);
        const Vec2 back = pos - (axis * bounds.radius);

        const float p0 = front.dot(axis);
        const float p1 = back.dot(axis);

        return {Calc::min(p0, p1), Calc::max(p0, p1)};
    }

    Vec2 CircleCollider::nearest_vertex(const Engine::Vec2& pos) const
    {
        return m_entity->pos + bounds.center;
    }

    Rect CircleCollider::bounding_box() const
    {
        const Vec2 pos = m_entity->pos + bounds.center - Vec2(1.0f, 1.0f) * bounds.radius;
        return Rect(pos, Vec2(1.0f, 1.0f) * 2 * bounds.radius);
    }

    void CircleCollider::render(Engine::Renderer* renderer)
    {
        Vec2 pos = bounds.offset(m_entity->pos).center;
        renderer->circ(bounds.offset(m_entity->pos), 128, color);
    }
}
