#include "sb/boxcollider.h"
#include "engine/maths/calc.h"
#include <assert.h>

namespace SB
{
    using namespace Engine;

    BoxCollider::BoxCollider(const Rect& bounds)
        : bounds(bounds)
    {}

    bool BoxCollider::contains(const Vec2& pos, const Vec2& point) const
    {
        Rect rect = bounds.offset(pos - (Vec2(bounds.w, bounds.h) / 2.0f));
        return rect.contains(point);
    }

    bool BoxCollider::intersects(const Vec2& pos, const Line& line) const
    {
        Rect rect = bounds.offset(pos - (Vec2(bounds.w, bounds.h) / 2.0f));
        return rect.intersects(line);
    }
    
    std::vector<Vec2> BoxCollider::axes(const Vec2& pos, const Vec2& other_pos) const
    {
        std::vector<Vec2> axes;
        axes.push_back(Vec2(1.0f, 0.0f));
        axes.push_back(Vec2(0.0f, 1.0f));

        return axes;
    }

    Collider::Projection BoxCollider::projection(const Vec2& pos, const Vec2& axis) const
    {
        Rect rect = bounds.offset(pos - (Vec2(bounds.w, bounds.h) / 2.0f));

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
        Rect r = bounds.offset(pos - (Vec2(bounds.w, bounds.h) / 2.0f));
        renderer->rect(r, Color::red);
    }
}
