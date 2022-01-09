#pragma once
#include "sb/entity.h"
#include "sb/player.h"
#include "sb/circlecollider.h"

namespace SB
{
    class Enemy : public Entity
    {
    private:
        CircleCollider m_collider;

        float m_shoot_cooldown_timer;

        static constexpr float bullet_speed = 40.0f;
        static constexpr float shoot_cooldown = 2.0f;

    public:
        Enemy(const Engine::Vec2& pos, const float radius);

        void update(const float elapsed) override;
        void render(Engine::Renderer* renderer) override;
    };
}
