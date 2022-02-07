#pragma once
#include "sb/collider.h"
#include "engine/maths/circ.h"

namespace SB
{
    class CircleCollider : public Collider
    {
    public:
        Engine::Circ bounds;

    public:
        CircleCollider(const Engine::Circ& bounds);

        Engine::Vec2 nearest_vertex(const Engine::Vec2& pos) const override;
        Engine::Vec2 pos() const override;

        void render(Engine::Renderer* renderer) override;

    protected:
        std::vector<Engine::Vec2> axes(const Collider& other) const override;
        Projection projection(const Engine::Vec2& axis) const override;
    };
}
