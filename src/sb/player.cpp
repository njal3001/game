#include "sb/player.h"
#include "engine/input.h"
#include "engine/maths/calc.h"
#include "sb/boxcollider.h"
#include "sb/bullet.h"
#include <vector>
#include "sb/enemy.h"

namespace SB
{
    using namespace Engine;

    Player::Player()
        : m_facing(Vec2(1.0f, 0.0f)), m_dash_timer(0.0f),
        m_dash_cooldown_timer(0.0f), m_dash_stopped(false), m_invincible_timer(0.0f)
    {}

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
        CircleCollider* collider = (CircleCollider*)get<Collider>();

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
                std::vector<Bullet*> bullets;
                scene()->all(&bullets);
                for (auto b : bullets)
                {
                    Collider* b_collider = b->get<Collider>();
                    if (b_collider->intersects(*collider))
                    {
                        b->entity()->destroy();
                        printf("Bullet destroyed!\n");
                    }
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

        m_entity->pos += m_vel * elapsed;

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

        std::vector<Enemy*> enemies;
        scene()->all(&enemies);

        for (auto e : enemies)
        {
            Collider* c = e->get<Collider>();

            /* Vec2 disp = collider->displace(*c); */
            /* printf("disp: (%f, %f)\n", disp.x, disp.y); */
            /* m_entity->pos += disp; */
            
            bool i = collider->Collider::intersects(*c);
            /* printf("intersects: %d\n", i); */
        }

        // Check bounds
        {
            const Rect bounds = scene()->bounds;

            const Vec2 center = collider->bounds.center;
            const float radius = collider->bounds.radius;
            m_entity->pos.x = Calc::clamp(bounds.x + center.x + radius,
                    bounds.x + center.x + bounds.w - radius, m_entity->pos.x); 

            m_entity->pos.y = Calc::clamp(bounds.y + center.y + radius,
                    bounds.y + center.y + bounds.h - radius, m_entity->pos.y); 
        }
    }

    void Player::render(Engine::Renderer* renderer)
    {
        const CircleCollider* collider = (CircleCollider*)get<Collider>();

        const Color c = m_dash_timer > 0.0f ? Color::blue : 
            (m_invincible_timer > 0.0f ? Color::white :
                (m_dash_cooldown_timer <= 0.0f ? Color::green : Color::red));

        const float radius = dashing() ? dash_shield_radius : collider->bounds.radius;

        renderer->circ(m_entity->pos, radius, 128, c);
    }

    Entity* Player::create(Scene* scene, const Engine::Vec2& pos)
    {
        Entity* e = scene->add_entity(pos);
        e->add(new Player());

        Collider* c = new CircleCollider(Circ(Vec2(), collider_radius));
        e->add(c);

        return e;
    }
}
