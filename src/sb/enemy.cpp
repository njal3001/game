#include "sb/enemy.h"
#include "sb/bullet.h"
#include "sb/boxcollider.h"
#include "sb/mover.h"

namespace SB
{
    using namespace Engine;

    Enemy::Enemy()
        : m_shoot_cooldown_timer(shoot_cooldown * ((float)(rand() / (float)RAND_MAX))),
        m_health(3), m_invincible_timer(0.0f)
    {}

    void Enemy::update(const float elapsed)
    {
        m_invincible_timer -= elapsed;

        // Check if hurt by player
        auto collider = (CircleCollider*)get<Collider>();
        if (m_invincible_timer <= 0.0f && collider->check(Mask::PlayerDash))
        {
            m_health -= 1;
            m_invincible_timer = invincible_time;
            m_shoot_cooldown_timer = shoot_cooldown;

            if (m_health <= 0)
            {
                m_entity->destroy();
            }

            printf("Hurt enemy!\n");
        }
        else
        {
            Player* player = scene()->first<Player>();

            if (player)
            {
                m_shoot_cooldown_timer -= elapsed;
                
                if (m_shoot_cooldown_timer <= 0.0f)
                {
                    Vec2 bullet_vel = (player->entity()->pos - m_entity->pos).norm() * bullet_speed;
                    Bullet::create(scene(), m_entity->pos, bullet_vel, 1.0f);

                    m_shoot_cooldown_timer = shoot_cooldown;
                }


                // Appoach player
                auto mover = get<Mover>();
                const Vec2 dir = (player->entity()->pos - m_entity->pos);
                if (dir.len_squared() > collider->bounds.radius + 10.0f)
                {
                    mover->vel = Vec2::approach(mover->vel, dir.norm() * 40.0f, 20.0f * elapsed);
                }
                else
                {
                    mover->vel = Vec2::approach(mover->vel, Vec2(), 20.0f * elapsed);
                }
            }
        }
    }

    Entity* Enemy::create(Scene* scene, const Engine::Vec2& pos, const float radius)
    {
        Entity* e = scene->add_entity(pos);
        e->add(new Enemy());

        Collider* c = new CircleCollider(Circ(Vec2(), radius));
        c->mask = Mask::Enemy | Mask::DashTrough;
        e->add(c);

        c->visible = true;
        c->color = Color::white;

        Mover* m = new Mover();
        m->collider = c;
        m->stop_mask |= Mask::Enemy | Mask::Player;
        e->add(m);

        return e;
    }
}
