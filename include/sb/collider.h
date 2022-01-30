#pragma once
#include <vector>
#include <functional>
#include "engine/maths/vec2.h"
#include "engine/maths/line.h"
#include "engine/graphics/renderer.h"
#include "sb/ecs.h"

namespace SB
{
    struct Mask
    {
        static constexpr uint32_t None = 0;
        static constexpr uint32_t Solid = 1;
        static constexpr uint32_t Enemy = 1 << 1;
        static constexpr uint32_t Player = 1 << 2;
        static constexpr uint32_t PlayerDash = 1 << 3;
        static constexpr uint32_t PlayerAttack = 1 << 4;
    };

    class Collider : public Component
    {
    protected:
        struct Projection
        {
            float start;
            float end;
        };

    public:
        uint32_t mask;
        bool trigger_only;
        Engine::Color color;

    protected:
        virtual std::vector<Engine::Vec2> axes(const Collider& other) const = 0;
        virtual Projection projection(const Engine::Vec2& axis) const = 0;

    public:
        Collider();

        virtual Engine::Vec2 nearest_vertex(const Engine::Vec2& pos) const = 0;

        bool intersects(const Collider& other) const;
        
        // Returns a vector that pushes this collider out of the other collider
        Engine::Vec2 displace(const Collider& other) const;

        float distance(const Collider& other) const;

        bool check(const uint32_t mask) const;
    };
}
