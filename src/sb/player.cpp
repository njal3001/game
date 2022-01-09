#include "sb/player.h"
#include "engine/input.h"
#include "engine/maths/calc.h"
#include "sb/scene.h"
#include "sb/boxcollider.h"

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

    void Player::awake()
    {
        m_scene->player = this;
    }

    void Player::destroy()
    {
        Entity::destroy();
        m_scene->player = nullptr;
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

        // Check bounds
        // TODO: Cleanup a bit
        {
            Rect bounds = m_scene->bounds;
            const float check_width = 1000.0f;

            // Y direction
            BoxCollider ycol(Vec2(bounds.w, check_width));

            Rect bottom(bounds.x, bounds.y - check_width, bounds.w, check_width);
            Vec2 disp = m_collider.Collider::static_displacement(pos, bottom.center(), ycol);

            if (disp != Vec2::zero)
            {
                pos += disp;
                m_vel.y = 0.0f;
            }
            else
            {
                Rect top(bounds.x, bounds.y + bounds.h, bounds.w, check_width);
                disp = m_collider.Collider::static_displacement(pos, top.center(), ycol);

                if (disp != Vec2::zero)
                {
                    pos += disp;
                    m_vel.y = 0.0f;
                }
            }

            // X direction
            BoxCollider xcol(Vec2(check_width, bounds.h));

            Rect left(bounds.x - check_width, bounds.y, check_width, bounds.h);
            disp = m_collider.Collider::static_displacement(pos, left.center(), xcol);

            if (disp != Vec2::zero)
            {
                pos += disp;
                m_vel.x = 0.0f;
            }
            else
            {
                Rect right(bounds.x + bounds.w, bounds.y, check_width, bounds.h);
                disp = m_collider.Collider::static_displacement(pos, right.center(), xcol);

                if (disp != Vec2::zero)
                {
                    pos += disp;
                    m_vel.x = 0.0f;
                }
            }
        }
    }

    void Player::render(Engine::Renderer* renderer)
    {
        Color c = m_dash_timer > 0.0f ? Color::blue : (m_dash_cooldown_timer <= 0.0f ? Color::green : Color::red);
        renderer->circ(pos, m_collider.radius, 128, c);
    }
}
