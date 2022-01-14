#pragma once
#include "sb/ecs.h"
#include "sb/player.h"
#include "sb/circlecollider.h"

namespace SB
{
    class Bullet : public Component
    {
    public:
        Engine::Vec2 vel;

    public:
        Bullet(const Engine::Vec2& vel);

        void update(const float elapsed) override;
        void render(Engine::Renderer* renderer) override;

        static Entity* create(Scene* scene, const Engine::Vec2& pos,
                const Engine::Vec2& vel, const float radius);
    };
}
