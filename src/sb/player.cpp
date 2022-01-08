#include "sb/player.h"
#include "engine/input.h"
#include "engine/maths/calc.h"

namespace SB
{
    using namespace Engine;

    Player::Player(const Vec2& pos)
        : Entity(pos), m_collider(CircleCollider(4.0f)), m_dash_timer(0.0f), 
        m_dash_cooldown_timer(0.0f), m_facing(Vec2(1.0f, 0.0f))
    {}

    const Collider* Player::collider() const
    {
        return &m_collider;
    }

    void Player::update(const float elapsed)
    {
        Vec2 dir;
        dir.x = Input::axis_state(Axis::LeftX);
        dir.y = -Input::axis_state(Axis::LeftY);

        if (dir.len() >= 0.99f)
        {
            dir = dir.norm();
        }

        if (dir.x || dir.y)
        {
            m_facing = dir;
        }

        if (m_dash_timer > 0.0f)
        {
            m_dash_timer -= elapsed;
        }
        else
        {
            m_dash_cooldown_timer -= elapsed;

            // TODO: Compare square instead
            if (m_vel.len() - 1.0f > max_swim_speed)
            {
                m_vel = Vec2::approach(m_vel, dir * max_swim_speed, swim_deaccel * elapsed);
            }
            else
            {
                m_vel = Vec2::approach(m_vel, dir * max_swim_speed, swim_accel * elapsed);
            }
        }

        pos += m_vel * elapsed;

        // Dash
        if (Input::controller_button_state(ControllerButton::A).pressed && m_dash_cooldown_timer <= 0.0f)
        {
            m_dash_timer = dash_time;
            m_dash_target = m_facing;
            m_vel = m_facing * dash_speed;
            m_dash_cooldown_timer = dash_cooldown;
        }
    }

    void Player::render(Engine::Renderer* renderer)
    {
        Color c = m_dash_timer > 0.0f ? Color::blue : (m_dash_cooldown_timer <= 0.0f ? Color::green : Color::red);
        renderer->circ(pos, m_collider.radius, 128, c);
    }
}
