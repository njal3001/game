#include "sb/player.h"
#include "engine/input.h"
#include "engine/maths/calc.h"
#include "sb/scene.h"
#include "sb/boxcollider.h"
#include "sb/bullet.h"
#include <vector>

namespace SB
{
    using namespace Engine;

    Player::Player(const Vec2& pos)
        : Entity(pos), m_collider(CircleCollider(Circ(Vec2(), collider_radius))), 
        m_facing(Vec2(1.0f, 0.0f)), m_dash_timer(0.0f), 
        m_dash_cooldown_timer(0.0f), m_dash_stopped(false), m_invincible_timer(0.0f)
    {}

    const Collider* Player::collider() const
    {
        return &m_collider;
    }

    void Player::hurt()
    {
        if (m_dash_timer <= 0.0f && m_invincible_timer <= 0.0f)
        {
            m_invincible_timer = invincible_time;
            printf("Tok damage!\n");
        }
    }
    
    bool Player::dashing() const
    {
        return m_dash_timer > 0.0f;
    }

    void Player::update(const float elapsed)
    {
        m_invincible_timer -= elapsed;

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
                m_facing = dir.norm();
            }

            if (m_dash_timer > 0.0f)
            {
                m_dash_timer -= elapsed;

                // Dash attack
                Bullet* b = m_scene->first<Bullet>();
                while (b)
                {
                    if (b->collider().intersects(b->pos, pos, m_collider))
                    {
                        b->destroy();
                        printf("Bullet destroyed!\n");
                    }
                    
                    b = (Bullet*)b->next();
                }

                // Stop dash early
                if (!m_dash_stopped && Input::controller_button_state(ControllerButton::A).released)
                {
                    m_dash_timer = dash_min_time - (dash_max_time - m_dash_timer);
                    m_dash_stopped = true;
                }
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
        }

        pos += m_vel * elapsed;

        // Dash
        if (Input::controller_button_state(ControllerButton::A).pressed 
                && m_dash_cooldown_timer <= 0.0f)
        {
            m_dash_timer = dash_max_time;
            m_dash_target = m_facing;
            m_vel = m_facing * dash_speed;
            m_dash_cooldown_timer = dash_cooldown;
            m_dash_stopped = false;
        }

        // Check bounds
        {
            const Rect bounds = m_scene->bounds;

            const Vec2 center = m_collider.bounds.center;
            const float radius = m_collider.bounds.radius;
            pos.x = Calc::clamp(bounds.x + center.x + radius,
                    bounds.x + center.x + bounds.w - radius, pos.x); 

            pos.y = Calc::clamp(bounds.y + center.y + radius,
                    bounds.y + center.y + bounds.h - radius, pos.y); 
        }
    }

    void Player::render(Engine::Renderer* renderer)
    {
        const Color c = m_dash_timer > 0.0f ? Color::blue : 
            (m_invincible_timer > 0.0f ? Color::white :
                (m_dash_cooldown_timer <= 0.0f ? Color::green : Color::red));

        const float radius = dashing() ? dash_shield_radius : m_collider.bounds.radius;
        renderer->circ(pos, radius, 128, c);
    }
}
