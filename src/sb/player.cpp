#include "sb/player.h"
#include "engine/input.h"
#include "engine/maths/calc.h"

namespace SB
{
    using namespace Engine;

    Player::Player(const Vec2& pos)
        : Entity(pos), m_collider(CircleCollider(4.0f)), m_stroke_timer(0.0f), m_stroke_cooldown_timer(0.0f)
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
        dir = dir.norm();

        // TODO: Build up momentum with multiple swim strokes
        if (m_stroke_timer <= 0.0f) 
        {
            m_vel.x = Calc::lerp(m_vel.x, 0.0f, deaccel);
            m_vel.y = Calc::lerp(m_vel.y, 0.0f, deaccel);
        }
        else
        {
            m_stroke_timer -= elapsed;
            
            m_vel.x = Calc::lerp(m_vel.x, m_stroke_dir.x * stroke_target_speed, stroke_accel);
            m_vel.y = Calc::lerp(m_vel.y, m_stroke_dir.y * stroke_target_speed, stroke_accel);
        }

        m_stroke_cooldown_timer -= elapsed;

        if (m_stroke_cooldown_timer <= 0.0f && Input::controller_button_state(ControllerButton::A).pressed)
        {
            if (dir.len_squared() > 0.0f)
            {
                m_stroke_dir = dir.norm();
                m_stroke_timer = stroke_duration;
                m_stroke_cooldown_timer = stroke_cooldown;
            }
            else
            {
                m_stroke_timer = 0.0f;
            }
        }

        pos += m_vel * elapsed;
    }

    void Player::render(Engine::Renderer* renderer)
    {
        Color c = m_stroke_timer <= 0.0f ? Color::green : Color::red; 
        renderer->circ(pos, m_collider.radius, 128, c);
    }
}
