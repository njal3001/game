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

        Engine::Vec2 m_stroke_dir;
        float m_stroke_timer;
        float m_stroke_cooldown_timer;

        // TODO: Should this be declared in cpp file instead?
        static constexpr float deaccel = 0.8f;
        static constexpr float stroke_accel = 8.0f;
        static constexpr float stroke_target_speed = 32.0f;
        static constexpr float stroke_duration = 0.5f;
        static constexpr float stroke_cooldown = 0.0f;

    public:
        Player(const Engine::Vec2& pos);

        const Collider* collider() const;

        void update(const float elapsed) override;
        void render(Engine::Renderer* renderer) override;
    };
}
