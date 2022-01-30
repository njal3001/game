#pragma once
#include "sb/ecs.h"

namespace SB
{
    class Charger : public Component
    {
    private:
        enum class State
        {
            Wander,
            Search,
            WindUp,
            Charge,
            Rest,
            Hurt
        };

        State m_state;
        Engine::Vec2 m_charge_dir;
        float m_state_timer;
        size_t m_health;
        float m_invincible_timer;

        static constexpr float accel = 68.0f;
        static constexpr float max_speed = 48.0f;

        static constexpr float charge_radius = 8.0f;
        static constexpr float charge_wind_up_time = 0.55f;
        static constexpr float charge_accel = 237.0f;
        static constexpr float charge_speed = 189.0f;
        static constexpr float charge_max_time = 0.4f;
        static constexpr float charge_min_time = 0.2f;

        static constexpr float rest_time = 0.8f;
        static constexpr float rest_deaccel = 267.0f;

        static constexpr float knockback_deaccel = 128.0f;
        static constexpr float knockback_speed = 85.0f;
        static constexpr size_t max_health = 5;
        static constexpr float knockback_time = 0.3f;
        static constexpr float invincible_time = 1.0f;

    public:
        Charger();

        void on_hit(Collider* other, const Engine::Vec2& dir);
        void hurt(const Engine::Vec2& attack_dir);
        void update(const float elapsed) override;

        static Entity* create(Scene* scene, const Engine::Vec2& pos, const float radius);
    };
}
