#include "sb/player.h"
#include "engine/input.h"
#include "engine/maths/calc.h"
#include "sb/boxcollider.h"
#include "sb/bullet.h"
#include <vector>
#include "sb/enemy.h"
#include "sb/mover.h"

namespace SB
{
    using namespace Engine;

    Player::Player()
        : m_state(State::Normal), m_facing(Vec2(1.0f, 0.0f)), m_dash_timer(0.0f), 
        m_dash_cooldown_timer(0.0f), m_invincible_timer(0.0f)
    {}

    void Player::hurt()
    {
        if (m_dash_timer <= 0.0f && m_invincible_timer <= 0.0f)
        {
            m_invincible_timer = invincible_time;
            printf("Tok damage!\n");
        }
    }
    
    void Player::update(const float elapsed)
    {
        auto collider = get<Collider>();
        auto mover = get<Mover>();

        m_invincible_timer -= elapsed;

        {
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

                    m_dash_cooldown_timer -= elapsed;

                    // Update speed
                    // TODO: Compare square instead
                    if (mover->vel.len() - 1.0f > max_speed)
                    {
                        // Decelerate if faster than max speed
                        mover->vel = Vec2::approach(mover->vel, dir * max_speed, 
                                deaccel * elapsed);
                    }
                    else
                    {
                        // Approach the max speed
                        mover->vel = Vec2::approach(mover->vel, dir * max_speed, 
                                accel * elapsed);
                    }
                    
                    // Start dash
                    if (Input::controller_button_state(ControllerButton::LeftShoulder).pressed 
                            && m_dash_cooldown_timer <= 0.0f)
                    {
                        collider->mask = Mask::PlayerDash;

                        m_dash_timer = dash_time;
                        mover->vel = m_facing * dash_speed;
                        mover->stop_on_collide = false;

                        m_state = State::Dash;
                    }

                    break;
                }
                case State::Dash:
                {
                    m_dash_timer -= elapsed;

                    // Stop dash if done
                    if (m_dash_timer <= 0.0f)
                    {
                        collider->mask = Mask::Player;
                        m_dash_cooldown_timer = dash_cooldown;
                        mover->stop_on_collide = true;

                        m_state = State::Normal;
                    }

                    break;
                }
            }
        }
    }

    void Player::render(Engine::Renderer* renderer)
    {
        const CircleCollider* collider = (CircleCollider*)get<Collider>();

        const Color c = m_dash_timer > 0.0f ? Color::blue : 
            (m_invincible_timer > 0.0f ? Color::white :
                (m_dash_cooldown_timer <= 0.0f ? Color::green : Color::red));

        renderer->circ(m_entity->pos, collider->bounds.radius, 128, c);
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
