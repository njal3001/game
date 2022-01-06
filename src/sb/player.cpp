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
        if (Input::key_state (Key::Left).down)                
        {
            dir.x -= 1;      
        }                    
        if (Input::key_state(Key::Right).down)
        {                    
            dir.x += 1;      
        }
        if (Input::key_state(Key::Down).down) 
        {
            dir.y -= 1;
        }
        if (Input::key_state(Key::Up).down)
        {
            dir.y += 1;
        }

        // TODO: Add controller support
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

        if (m_stroke_cooldown_timer <= 0.0f && Input::key_state(Key::X).pressed)
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
        m_collider.render(pos, renderer);
    }
}
