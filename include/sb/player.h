#pragma once
#include "sb/ecs.h"
#include "sb/boxcollider.h"
#include "sb/mover.h"

namespace SB
{
    class Player : public Component
    {
    private:
        enum class State
        {
            Normal,
            Dash
        };

        State m_state;
        BoxCollider* m_collider;
        Mover* m_mover;

        Engine::Vec2 m_facing;

        float m_dash_timer;
        float m_dash_cooldown_timer;

        float m_shoot_cooldown_timer;

        float m_invincible_timer;
        size_t m_health;

        static constexpr float collider_width = 8.0f;
        static constexpr float collider_height = 6.0f;
        static constexpr float max_speed = 68.0f;
        static constexpr float accel = 278.0f;
        static constexpr float deaccel = 427.0f;

        static constexpr float dash_speed = 126.0f;
        static constexpr float dash_cooldown = 0.6f;
        static constexpr float dash_time = 0.24f;

        static constexpr float bullet_speed = 147.0f;
        static constexpr float bullet_lifetime = 0.3f;
        static constexpr float bullet_radius = 2.0f;
        static constexpr float bullet_offset = 2.5f;
        static constexpr float shoot_cooldown = 0.8f;

        static constexpr size_t max_health = 5;
        static constexpr float invincible_time = 1.0f;

    public:
        Player();

        void hurt();

        void awake() override;
        void update(const float elapsed) override;
        void render(Engine::Renderer* renderer) override;

        void start_dash();
        void stop_dash();

        static Entity* create(Scene* scene, const Engine::Vec2& pos);
    };
}
