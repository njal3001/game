#pragma once
#include "sb/collider.h"
#include "engine/maths/rect.h"

namespace SB
{
    class BoxCollider : public Collider
    {
    public:
        Engine::Rect rect;

    public:
        BoxCollider(const Engine::Rect& rect);

        Engine::Vec2 center() const override;
        void move(const Engine::Vec2& amount) override;


    protected:
        std::vector<Engine::Vec2> get_axes(const Collider& other) const override;
        Projection get_projection(const Engine::Vec2& axis) const override;
    };
}
