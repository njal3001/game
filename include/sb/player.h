#pragma once
#include "sb/ecs.h"
#include "sb/circlecollider.h"

namespace SB
{
    class Player : public Component
    {
    private:
        Engine::Vec2 m_facing;

        CircleCollider* m_dash_collider;
        bool m_is_dashing;
        float m_dash_timer;
        float m_dash_cooldown_timer;
        bool m_dash_stopped;

        float m_invincible_timer;

        static constexpr float collider_radius = 4.0f;
        static constexpr float max_swim_speed = 68.0f;
        static constexpr float swim_accel = 278.0f;
        static constexpr float swim_deaccel = 427.0f;

        static constexpr float dash_speed = 126.0f;
        static constexpr float dash_cooldown = 1.0f;
        static constexpr float dash_min_time = 0.1f;
        static constexpr float dash_max_time = 0.25f;
        static constexpr float dash_shield_radius = 8.0f;

        static constexpr float invincible_time = 1.0f;

    public:
        Player();

        void hurt();
        bool dashing() const;

        void on_hit(const Engine::Vec2& dir, const Engine::Vec2& prev_vel);

        void awake() override;
        void update(const float elapsed) override;
        void render(Engine::Renderer* renderer) override;

        static Entity* create(Scene* scene, const Engine::Vec2& pos);
    };
}
