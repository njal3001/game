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
        : m_facing(Vec2(1.0f, 0.0f)), m_dash_collider(nullptr), m_is_dashing(false), m_dash_timer(0.0f),
        m_dash_cooldown_timer(0.0f), m_dash_stopped(false), m_invincible_timer(0.0f)
    {
    }

    void Player::hurt()
    {
        if (!m_is_dashing && m_invincible_timer <= 0.0f)
        {
            m_invincible_timer = invincible_time;
            printf("Tok damage!\n");
        }
    }
    
    bool Player::dashing() const
    {
        return m_is_dashing;
    }

    void Player::on_hit(const Vec2& dir, const Vec2& prev_vel)
    {
        if (m_is_dashing)
        {
            auto mover = get<Mover>();
            Vec2 reflected = prev_vel - (dir * prev_vel.dot(dir) * 2.0f);
            mover->vel = reflected;
        }
    }

    void Player::awake()
    {
        m_dash_collider = new CircleCollider(Circ(Vec2(), dash_shield_radius));

        // Note: Need to cast to Collider here to avoid 
        // that the circle collider is treated as it's own type
        m_entity->add((Collider*)m_dash_collider);
    }

    void Player::update(const float elapsed)
    {
        auto collider = get<Collider>();
        auto mover = get<Mover>();

        std::vector<Collider*> out;
        scene()->all(&out, Mask::PlayerDash);

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

            if (m_is_dashing)
            {
                m_dash_timer -= elapsed;

                // Stop dash early
                if (!m_dash_stopped && Input::controller_button_state(ControllerButton::A).released)
                {
                    m_dash_timer = dash_min_time - (dash_max_time - m_dash_timer);
                    m_dash_stopped = true;
                }

                // Stop dash
                if (m_dash_timer <= 0.0f && !collider->check(Mask::Enemy))
                {
                    m_is_dashing = false;

                    collider->mask = Mask::Player;
                    mover->stop_mask |= Mask::Enemy;
                    m_dash_collider->mask = Mask::None;

                    m_dash_cooldown_timer = dash_cooldown;
                }
            }
            else
            {
                // Not dashing
                m_dash_cooldown_timer -= elapsed;

                // TODO: Compare square instead
                if (mover->vel.len() - 1.0f > max_swim_speed)
                {
                    mover->vel = Vec2::approach(mover->vel, dir * max_swim_speed, swim_deaccel * elapsed);
                }
                else
                {
                    mover->vel = Vec2::approach(mover->vel, dir * max_swim_speed, swim_accel * elapsed);
                }

                // Start dash
                if (Input::controller_button_state(ControllerButton::A).pressed 
                        && m_dash_cooldown_timer <= 0.0f)
                {
                    m_is_dashing = true;

                    // Disable player collisions
                    collider->mask = Mask::None;

                    // Dash through enemies
                    mover->stop_mask ^= Mask::Enemy;

                    // Activate dash collider
                    m_dash_collider->mask = Mask::PlayerDash;

                    m_dash_timer = dash_max_time;
                    mover->vel = m_facing * dash_speed;
                    m_dash_stopped = false;
                }
            }
        }
    }

    void Player::render(Engine::Renderer* renderer)
    {
        const CircleCollider* collider = (CircleCollider*)get<Collider>();

        const Color c = m_is_dashing ? Color::blue : 
            (m_invincible_timer > 0.0f ? Color::white :
                (m_dash_cooldown_timer <= 0.0f ? Color::green : Color::red));

        const float radius = m_is_dashing ? dash_shield_radius : collider->bounds.radius;

        renderer->circ(m_entity->pos, radius, 128, c);
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

        m->on_hit = [](Mover* mover, Collider* other, const Vec2& dir, const Vec2& prev_vel)
        {
            Player* p = mover->get<Player>();
            p->on_hit(dir, prev_vel);
        };

        e->add(m);

        return e;
    }
}
