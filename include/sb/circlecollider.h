#pragma once
#include "sb/collider.h"
#include "engine/maths/circ.h"

namespace SB
{
    class CircleCollider : public Collider
    {
    public:
        float radius;

    public:
        CircleCollider(const float radius);

        bool contains(const Engine::Vec2& pos, const Engine::Vec2& point) const override;
        bool intersects(const Engine::Vec2& pos, const Engine::Line& line) const override;

        void draw(const Engine::Vec2& pos, Engine::Renderer* renderer) const override;

    protected:
        std::vector<Engine::Vec2> get_axes(const Engine::Vec2& pos,
                const Engine::Vec2& pos_other) const override;
        Projection get_projection(const Engine::Vec2& pos, const Engine::Vec2& axis) const override;
    };
}
