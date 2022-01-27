#pragma once
#include "engine/graphics/renderer.h"
#include "sb/collider.h"
#include "engine/maths/rect.h"

namespace SB
{
    class BoxCollider : public Collider
    {
    public:
        Engine::Rect bounds;
        float rotation; // In radians

    public:
        BoxCollider(const Engine::Rect& bounds);
        BoxCollider(const Engine::Rect& bounds, const float rotation);

        Engine::Vec2 nearest_vertex(const Engine::Vec2& pos) const override;

        void render(Engine::Renderer* renderer) override;

    protected:
        std::vector<Engine::Vec2> axes(const Collider& other) const override;
        Projection projection(const Engine::Vec2& axis) const override;
    };
}
