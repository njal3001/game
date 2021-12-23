#pragma once
#include "sb/collider.h"
#include "engine/maths/circ.h"

namespace SB
{
    class CircleCollider : public Collider
    {
    public:
        Engine::Circ circ;

    public:
        CircleCollider(const Engine::Circ& circ);

        Engine::Vec2 center() const override;
        void move(const Engine::Vec2& amount) override;

    protected:
        std::vector<Engine::Vec2> get_axes(const Collider& other) const override;
        Projection get_projection(const Engine::Vec2& axis) const override;
    };
}
