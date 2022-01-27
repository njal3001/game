#pragma once
#include "engine/maths/rect.h"
#include "sb/ecs.h"
#include "sb/circlecollider.h"
#include "sb/boxcollider.h"

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

        Engine::Vec2 m_facing;

        CircleCollider* m_weapon_collider;
        float m_dash_timer;
        float m_dash_cooldown_timer;

        float m_invincible_timer;

        static constexpr float collider_radius = 4.0f;
        static constexpr float max_speed = 68.0f;
        static constexpr float accel = 278.0f;
        static constexpr float deaccel = 427.0f;

        static constexpr float dash_speed = 126.0f;
        static constexpr float dash_cooldown = 0.6f;
        static constexpr float dash_time = 0.24f;

        static constexpr float invincible_time = 1.0f;

    public:
        Player();

        void hurt();

        void update(const float elapsed) override;
        void render(Engine::Renderer* renderer) override;

        static Entity* create(Scene* scene, const Engine::Vec2& pos);
    };
}
