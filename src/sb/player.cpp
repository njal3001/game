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
        m_facing(Vec2(1.0f, 0.0f)), m_dash_timer(0.0f), 
        m_dash_cooldown_timer(0.0f), m_attack_collider(nullptr), 
        m_attack_timer(0.0f), m_attack_cooldown_timer(0.0f), 
        m_invincible_timer(0.0f), m_health(max_health)
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
        m_collider = (CircleCollider*)get<Collider>();
        m_mover = get<Mover>();

        m_attack_collider = new CircleCollider(Circ(Vec2(), attack_radius));
        m_attack_collider->trigger_only = true;
        m_attack_collider->color = Color::red;
        m_entity->add((Collider*)m_attack_collider);
    }
    
    void Player::update(const float elapsed)
    {
        m_invincible_timer -= elapsed;
        m_dash_cooldown_timer -= elapsed;
        m_attack_cooldown_timer -= elapsed;

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

                // Attacking
                if (m_attack_timer > 0.0f)
                {
                    m_attack_timer -= elapsed;

                    if (m_attack_timer <= 0.0f)
                    {
                        stop_attack();
                    }
                }
                
                // Check for dash or attack input
                if (Input::controller_button_state(ControllerButton::LeftShoulder).pressed 
                        && m_dash_cooldown_timer <= 0.0f)
                {
                    // Cancel attack
                    if (m_attack_timer > 0.0f)
                    {
                        stop_attack();
                    }

                    start_dash();
                }
                else if (Input::controller_button_state(ControllerButton::RightShoulder).
                        pressed && m_attack_cooldown_timer <= 0.0f)
                {
                    start_attack();
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

    void Player::start_attack()
    {
        m_attack_timer = attack_time;
        m_attack_cooldown_timer = attack_cooldown;
        m_attack_collider->mask = Mask::PlayerAttack;
        m_attack_collider->visible = true;

        m_attack_collider->bounds.center = m_facing * attack_offset;
    }

    void Player::stop_attack()
    {
        m_attack_collider->visible = false;
        m_attack_collider->mask = Mask::None;
        m_state = State::Normal;
    }

    void Player::render(Engine::Renderer* renderer)
    {
        const Color c = m_state == State::Dash ? Color::blue : 
            (m_invincible_timer > 0.0f ? Color::white :
                (m_dash_cooldown_timer <= 0.0f ? Color::green : Color::red));

        renderer->circ(m_entity->pos, m_collider->bounds.radius, 128, c);
    }

    Entity* Player::create(Scene* scene, const Engine::Vec2& pos)
    {
        Entity* e = scene->add_entity(pos);
        e->add(new Player());

        Collider* c = new CircleCollider(Circ(Vec2(), collider_radius));
        e->add(c);

        Mover* m = new Mover();
        m->collider = c;
        c->mask = Mask::Player;
        m->stop_mask |= Mask::Enemy; 

        e->add(m);

        return e;
    }
}
