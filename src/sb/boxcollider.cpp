#include "sb/boxcollider.h"
#include "engine/maths/calc.h"
#include <assert.h>

namespace SB
{
    using namespace Engine;

    BoxCollider::BoxCollider(const Engine::Vec2& size)
        : size(size)
    {}

    Rect BoxCollider::make_rect(const Vec2& pos) const
    {
        Rect rect(pos.x - (size.x / 2.0f), pos.y - (size.y / 2.0f), size.x, size.y);
        return rect;
    }

    bool BoxCollider::contains(const Engine::Vec2& pos, const Engine::Vec2& point) const
    {
        Rect rect = make_rect(pos);
        return rect.contains(point);
    }

    bool BoxCollider::intersects(const Engine::Vec2& pos, const Engine::Line& line) const
    {
        Rect rect = make_rect(pos);
        return rect.intersects(line);
    }
    
    // TODO: Cache axes
    std::vector<Vec2> BoxCollider::get_axes(const Vec2& pos, const Vec2& other_pos) const
    {
        std::vector<Vec2> axes;
        axes.push_back(Vec2(1.0f, 0.0f));
        axes.push_back(Vec2(0.0f, 1.0f));

        return axes;
    }

    // TODO: Optimize
    Collider::Projection BoxCollider::get_projection(const Vec2& pos, const Vec2& axis) const
    {
        Rect rect(pos.x - (size.x / 2.0f), pos.y - (size.y / 2.0f), size.x, size.y);

        const float p0 = rect.top_left().dot(axis);
        const float p1 = rect.bottom_left().dot(axis);
        const float p2 = rect.top_right().dot(axis);
        const float p3 = rect.bottom_right().dot(axis);

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

    void BoxCollider::render(const Vec2& pos, Renderer* renderer) const
    {
        Rect r = make_rect(pos);
        renderer->rect(r, Color::red);
    }
}
