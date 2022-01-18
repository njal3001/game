#pragma once
#include "engine/maths/vec2.h"
#include "sb/player.h"
#include "sb/circlecollider.h"

namespace SB
{
    class Enemy : public Component
    {
    private:
        float m_shoot_cooldown_timer;
        int m_health;
        float m_invincible_timer;

        static constexpr float bullet_speed = 80.0f;
        static constexpr float shoot_cooldown = 2.0f;
        static constexpr float invincible_time = 1.0f;

    public:
        Enemy();

        void update(const float elapsed) override;

        static Entity* create(Scene* scene, const Engine::Vec2& pos, const float radius);
    };
}
