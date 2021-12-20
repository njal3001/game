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
        Engine::Vec2 center() const override;
        BoxCollider(const Engine::Rect& rect);

    protected:
        std::vector<Engine::Vec2> get_axes(const Collider& other) const override;
        Projection get_projection(const Engine::Vec2& axis) const override;
    };
}
