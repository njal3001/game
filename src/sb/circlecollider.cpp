#include "sb/circlecollider.h"
#include "engine/maths/calc.h"
#include <assert.h>

namespace SB
{
    using namespace Engine;

    CircleCollider::CircleCollider(const Engine::Circ& bounds)
        : bounds(bounds)
    {}

    bool CircleCollider::contains(const Engine::Vec2& pos, const Engine::Vec2& point) const
    {
        Circ circ(pos + bounds.center, bounds.radius);
        return circ.contains(point);
    }

    bool CircleCollider::intersects(const Engine::Vec2& pos, const Engine::Line& line) const
    {
        Circ circ(pos + bounds.center, bounds.radius);
        return circ.intersects(line);
    }

    std::vector<Vec2> CircleCollider::axes(const Vec2& pos, const Vec2& other_pos) const
    {
        std::vector<Vec2> axes;
        Vec2 diff = bounds.center + pos - other_pos;
        axes.push_back(diff.norm());

        return axes;
    }

    Collider::Projection CircleCollider::projection(const Vec2& pos, const Vec2& axis) const
    {
        const Vec2 front = bounds.center + pos + (axis * bounds.radius);
        const Vec2 back = bounds.center + pos - (axis * bounds.radius);

        float p0 = front.dot(axis);
        float p1 = back.dot(axis);

        // TODO: Could do with one if else instead
        return {Calc::min(p0, p1), Calc::max(p0, p1)};
    }

    void CircleCollider::render(const Vec2& pos, Renderer* renderer) const
    {
        renderer->circ(bounds.center + pos, bounds.radius, 128, Color::red);
    }
}
