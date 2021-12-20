#include "sb/boxcollider.h"
#include "engine/maths/calc.h"

namespace SB
{
    using namespace Engine;

    BoxCollider::BoxCollider(const Rect& rect)
        : rect(rect)
    {
        m_shape = &this->rect;
    }

    Vec2 BoxCollider::center() const
    {
        return rect.center();
    }
    
    // TODO: Cache axes
    std::vector<Vec2> BoxCollider::get_axes(const Collider& other) const
    {
        std::vector<Vec2> axes;
        axes.push_back(rect.top_right() - rect.top_left());
        axes.push_back(rect.top_left() - rect.bottom_left());

        return axes;
    }

    // TODO: Optimize
    Collider::Projection BoxCollider::get_projection(const Vec2& axis) const
    {
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
}
