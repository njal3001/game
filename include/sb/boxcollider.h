#pragma once
#include "sb/collider.h"
#include "engine/maths/rect.h"

namespace SB
{
    class BoxCollider : public Collider
    {
    public:
        Engine::Rect bounds;

    public:
        BoxCollider(const Engine::Rect& bounds);

        bool contains(const Engine::Vec2& pos, const Engine::Vec2& point) const override;
        bool intersects(const Engine::Vec2& pos, const Engine::Line& line) const override;

        void render(const Engine::Vec2& pos, Engine::Renderer* renderer) const override;

    protected:
        std::vector<Engine::Vec2> axes(const Engine::Vec2& pos,
                const Engine::Vec2& pos_other) const override;
        Projection projection(const Engine::Vec2& pos, const Engine::Vec2& axis) const override;
    };
}
