#include "sb/charger.h"
#include "sb/circlecollider.h"
#include "sb/mover.h"
#include "sb/player.h"

namespace SB
{
    using namespace Engine;

    Charger::Charger()
        : m_state(State::Search), m_state_timer(0.0f), m_health(max_health), 
        m_invincible_timer(0.0f)
    {}

    void Charger::on_hit(Collider* other, const Vec2& dir)
    {
        if (m_state == State::Charge)
        {
            if (other->mask & Mask::Player)
            {
                auto player = other->get<Player>();
                player->hurt();
            }

            if (!(other->mask & (Mask::Enemy | Mask::PlayerAttack)))
            {
                // Change to rest state
                m_state_timer = rest_time;
                m_state = State::Rest;
            }
        }
        else if (m_invincible_timer <= 0.0f && other->mask & Mask::PlayerAttack)
        {
            hurt(dir);
        }
    }

    void Charger::hurt(const Vec2& attack_dir)
    {
        m_health--;
        if (m_health == 0)
        {
            m_entity->destroy();
        }
        else
        {
            m_state_timer = knockback_time;
            m_invincible_timer = invincible_time;
            auto mover = get<Mover>();
            mover->vel = attack_dir * knockback_speed;
            m_state = State::Hurt;
        }
    }

    void Charger::update(const float elapsed)
     {
        auto collider = get<Collider>();
        auto mover = get<Mover>();
        Player* player = scene()->first<Player>();

        m_state_timer -= elapsed;
        m_invincible_timer -= elapsed;

        if (!player)
        {
            m_state = State::Wander;
        }

        switch (m_state)
        {
            case State::Wander:
            {
                mover->vel = Vec2::approach(mover->vel, Vec2(), 
                        accel * elapsed);

                break;
            }
            case State::Search:
            {
                // Appoach player
                const Vec2 dir = (player->entity()->pos - m_entity->pos).norm();
                mover->vel = Vec2::approach(mover->vel, dir * max_speed, 
                        accel * elapsed);

                // Check distance to player
                if (player->get<Collider>()->distance(*collider) <= charge_radius)
                {
                    // Begin charge wind up
                    m_state_timer = charge_wind_up_time;
                    m_state = State::WindUp;
                }

                break;
            }
            case State::WindUp:
            {
                mover->vel = Vec2::approach(mover->vel, Vec2(), 
                        accel * elapsed);

                // Start charge
                if (m_state_timer < 0.0f)
                {
                    const Vec2 dir = (player->entity()->pos - m_entity->pos).norm();
                    m_charge_dir = dir;

                    m_state_timer = charge_max_time;
                    m_state = State::Charge;
                } 

                break;
            }
            case State::Charge:
            {
                mover->vel = Vec2::approach(mover->vel, m_charge_dir * charge_speed, 
                        charge_accel * elapsed);

                const Vec2 pdir = player->entity()->pos - m_entity->pos;

                if (m_state_timer > charge_min_time && pdir.dot(m_charge_dir) < 0.0f)
                {
                    m_state_timer = charge_min_time;
                }

                if (m_state_timer < 0.0f)
                {
                    m_state_timer = rest_time;
                    m_state = State::Rest;
                }

                break;
            }
            case State::Rest:
            {
                mover->vel = Vec2::approach(mover->vel, Vec2(), 
                        rest_deaccel * elapsed);

                // Stop charge if timed out or if missed player
                if (m_state_timer < 0.0f)
                {
                    m_state = State::Search;
                }

                break;
            }
            case State::Hurt:
            {
                mover->vel = Vec2::approach(mover->vel, Vec2(), 
                        knockback_deaccel * elapsed);

                if (m_state_timer <= 0.0f)
                {
                    m_state = State::Search;
                }
            }
        }

        const Color c = 
            m_state == State::Search ? Color::green : (
                    m_state == State::WindUp ? Color::blue : (
                        m_state == State::Charge ? Color::red : Color::white));

        collider->color = c;
    }


    Entity* Charger::create(Scene* scene, const Engine::Vec2& pos, const float radius)
    {
        Entity* e = scene->add_entity(pos);
        e->add(new Charger());

        Collider* c = new CircleCollider(Circ(Vec2(), radius));
        c->visible = true;
        c->mask = Mask::Enemy;
        e->add(c);

        c->visible = true;
        c->color = Color::white;

        Mover* m = new Mover();
        m->collider = c;
        m->stop_mask |= Mask::Enemy | Mask::Player | Mask::PlayerDash | Mask::PlayerAttack;

        m->on_hit = [](Mover* mover, Collider* other, const Vec2& dir)
        {
            auto charger = mover->get<Charger>();
            charger->on_hit(other, dir);
        };

        e->add(m);

        return e;

    }
}
