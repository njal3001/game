#include "sb/player.h"
#include <vector>
#include "engine/input.h"
#include "engine/maths/calc.h"
#include "sb/boxcollider.h"
#include "sb/bullet.h"
#include "sb/enemy.h"

namespace SB
{
    using namespace Engine;

    Player::Player()
        : m_state(State::Normal), m_collider(nullptr), m_mover(nullptr), 
        m_facing(Vec2(1.0f, 0.0f)), m_dash_timer(0.0f), m_dash_cooldown_timer(0.0f),
        m_shoot_cooldown_timer(0.0f), m_invincible_timer(0.0f), m_health(max_health)
    {}

    void Player::hurt()
    {
        if (m_dash_timer <= 0.0f && m_invincible_timer <= 0.0f)
        {
            m_invincible_timer = invincible_time;
            m_health--;

            printf("Health: %d\n", (int)m_health);

            if (m_health == 0)
            {
                m_entity->destroy();
            }
        }
    }

    void Player::awake()
    {
        m_collider = (BoxCollider*)get<Collider>();
        m_mover = get<Mover>();
    }
    
    void Player::update(const float elapsed)
    {
        m_invincible_timer -= elapsed;
        m_dash_cooldown_timer -= elapsed;
        m_shoot_cooldown_timer -= elapsed;

        switch (m_state)
        {
            case State::Normal:
            {
                Vec2 dir;
                dir.x = Input::axis_state(Axis::LeftX);
                dir.y = -Input::axis_state(Axis::LeftY);

                // Clamp dir vector if longer than 1
                if (dir.len() > 1.0f)
                {
                    dir = dir.norm();
                }

                // Update facing if direction changed
                if (dir.x || dir.y)
                {
                    m_facing = dir.norm();
                    // TODO: No idea why this is correct
                    m_collider->rotation = -m_facing.angle(Vec2(1.0f, 0.0f));
                }

                // Update speed
                // TODO: Compare square instead
                if (m_mover->vel.len() - 1.0f > max_speed)
                {
                    // Decelerate if faster than max speed
                    m_mover->vel = Vec2::approach(m_mover->vel, dir * max_speed, 
                            deaccel * elapsed);
                }
                else
                {
                    // Approach the max speed
                    m_mover->vel = Vec2::approach(m_mover->vel, dir * max_speed, 
                            accel * elapsed);
                }

                // Check for dash or shoot input
                if (Input::controller_button_state(ControllerButton::LeftShoulder).pressed 
                        && m_dash_cooldown_timer <= 0.0f)
                {
                    start_dash();
                }
                else if (Input::controller_button_state(ControllerButton::RightShoulder).pressed 
                        && m_shoot_cooldown_timer <= 0.0f)
                {
                    Vec2 shoot_dir;
                    shoot_dir.x = Input::axis_state(Axis::RightX);
                    shoot_dir.y = -Input::axis_state(Axis::RightY);

                    if (shoot_dir.x || shoot_dir.y)
                    {
                        shoot_dir = shoot_dir.norm();
                    }
                    else
                    {
                        shoot_dir = m_facing;
                    }

                    Vec2 bullet_vel = shoot_dir * bullet_speed;
                    Bullet::create(scene(), m_entity->pos + m_facing * bullet_offset,
                            bullet_vel, bullet_lifetime, bullet_radius);

                    m_shoot_cooldown_timer = shoot_cooldown;
                }

                break;
            }
            case State::Dash:
            {
                m_dash_timer -= elapsed;

                if (m_dash_timer <= 0.0f)
                {
                    stop_dash();
                }

                break;
            }
        }
    }

    void Player::start_dash()
    {
        m_collider->mask = Mask::PlayerDash;

        m_dash_timer = dash_time;
        m_mover->vel = m_facing * dash_speed;
        m_mover->stop_on_collide = false;

        m_state = State::Dash;
    }

    void Player::stop_dash()
    {
        m_collider->mask = Mask::Player;
        m_dash_cooldown_timer = dash_cooldown;
        m_mover->stop_on_collide = true;

        m_state = State::Normal;
    }

    void Player::render(Engine::Renderer* renderer)
    {
        const Color c = m_state == State::Dash ? Color::blue : 
            (m_invincible_timer > 0.0f ? Color::white :
                (m_dash_cooldown_timer <= 0.0f ? Color::green : Color::red));

        const Rect r = m_collider->bounds.offset(m_entity->pos - 
                (Vec2(m_collider->bounds.w, m_collider->bounds.h) / 2.0f));
        const Quad q(r, m_collider->rotation);
        renderer->quad(q, c);
    }

    Entity* Player::create(Scene* scene, const Engine::Vec2& pos)
    {
        Entity* e = scene->add_entity(pos);
        e->add(new Player());

        Collider* c = new BoxCollider(Rect(0.0f, 0.0f, collider_width, collider_height));
        e->add(c);

        Mover* m = new Mover();
        m->collider = c;
        c->mask = Mask::Player;
        m->stop_mask |= Mask::Enemy; 

        e->add(m);

        return e;
    }
}
