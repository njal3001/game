#include "sb/circlecollider.h"
#include "engine/maths/calc.h"
#include <assert.h>

namespace SB
{
    using namespace Engine;

    CircleCollider::CircleCollider(const float radius)
        : radius(radius)
    {}

    bool CircleCollider::contains(const Engine::Vec2& pos, const Engine::Vec2& point) const
    {
        Circ circ(pos, radius);
        return circ.contains(point);
    }

    bool CircleCollider::intersects(const Engine::Vec2& pos, const Engine::Line& line) const
    {
        Circ circ(pos, radius);
        return circ.intersects(line);
    }

    std::vector<Vec2> CircleCollider::get_axes(const Vec2& pos, const Vec2& other_pos) const
    {
        std::vector<Vec2> axes;
        Vec2 diff = pos - other_pos;
        axes.push_back(diff.norm());

        return axes;
    }

    Collider::Projection CircleCollider::get_projection(const Vec2& pos, const Vec2& axis) const
    {
        const Vec2 front = pos + (axis * radius);
        const Vec2 back = pos - (axis * radius);

        float p0 = front.dot(axis);
        float p1 = back.dot(axis);

        // TODO: Could do with one if else instead
        return {Calc::min(p0, p1), Calc::max(p0, p1)};
    }

    void CircleCollider::draw(const Vec2& pos, Renderer* renderer) const
    {
        renderer->circ(pos, radius, 128, Color::red);
    }
}
