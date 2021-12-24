#pragma once
#include "sb/collider.h"
#include "engine/maths/rect.h"

namespace SB
{
    class BoxCollider : public Collider
    {
    public:
        Engine::Vec2 size;

    public:
        BoxCollider(const Engine::Vec2& size);

        bool contains(const Engine::Vec2& pos, const Engine::Vec2& point) const;
        bool intersects(const Engine::Vec2& pos, const Engine::Line& line) const;

    protected:
        std::vector<Engine::Vec2> get_axes(const Engine::Vec2& pos,
                const Engine::Vec2& pos_other) const;
        Projection get_projection(const Engine::Vec2& pos, const Engine::Vec2& axis) const;
    };
}
