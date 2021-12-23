#include "sb/circlecollider.h"
#include "engine/maths/calc.h"

namespace SB
{
    using namespace Engine;

    CircleCollider::CircleCollider(const Circ& circ)
        : circ(circ)
    {
        m_shape = &this->circ;
    }

    Vec2 CircleCollider::center() const
    {
        return circ.center;
    }
    
    void CircleCollider::move(const Engine::Vec2& amount)
    {
        circ.center += amount;
    }

    std::vector<Vec2> CircleCollider::get_axes(const Collider& other) const
    {
        std::vector<Vec2> axes;
        axes.push_back(circ.center - other.center());

        return axes;
    }

    Collider::Projection CircleCollider::get_projection(const Vec2& axis) const
    {
        const Vec2 norm = axis.norm();

        const Vec2 front = circ.center + (norm * circ.radius);
        const Vec2 back = circ.center - (norm * circ.radius);

        float p0 = front.dot(axis);
        float p1 = back.dot(axis);

        // TODO: Could do with one if else instead
        return {Calc::min(p0, p1), Calc::max(p0, p1)};
    }
}
