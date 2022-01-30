#pragma once
#include "sb/ecs.h"
#include <functional>

namespace SB
{
    class Mover : public Component
    {
    private:
        static constexpr float collision_elasticity = 0.01f;

    public:
        Collider* collider;
        Engine::Vec2 vel;
        bool stop_on_collide;
        uint32_t stop_mask;
        std::function<void (Mover* mover, Collider* other, const Engine::Vec2& dir)> on_hit;

        Mover();

        void update(const float elapsed) override;
    };
}
