#pragma once
#include "sb/entity.h"
#include "sb/circlecollider.h"

namespace SB
{
    class Player : public Entity
    {
    private:
        CircleCollider m_collider;
        Engine::Vec2 m_vel;

        float m_dash_timer;
        float m_dash_cooldown_timer;
        Engine::Vec2 m_dash_target;
        Engine::Vec2 m_facing;

        static constexpr float max_swim_speed = 68.0f;
        static constexpr float swim_accel = 278.0f;
        static constexpr float swim_deaccel = 312.0f;

        static constexpr float dash_speed = 126.0f;
        static constexpr float dash_time = 0.3f;
        static constexpr float dash_cooldown = 1.0f;

    public:
        Player(const Engine::Vec2& pos);

        const Collider* collider() const;

        void update(const float elapsed) override;
        void render(Engine::Renderer* renderer) override;
    };
}
