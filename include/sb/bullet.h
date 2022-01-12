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

    private:
        CircleCollider m_collider;

    public:
        Bullet(const Engine::Vec2& vel, const float radius);

        const Collider& collider() const;

        void update(const float elapsed) override;
        void render(Engine::Renderer* renderer) override;
    };
}
