#include "sb/enemy.h"
#include "sb/bullet.h"
#include "sb/boxcollider.h"
#include "sb/mover.h"
#include <vector>
#include <cmath>

namespace SB
{
    using namespace Engine;

    Enemy::Enemy()
        : m_shoot_cooldown_timer(shoot_cooldown * ((float)(rand() / (float)RAND_MAX)))
    {}

    void Enemy::update(const float elapsed)
    {
        Player* player = scene()->first<Player>();

        if (player)
        {
            m_shoot_cooldown_timer -= elapsed;
            
            // Shoot
            if (m_shoot_cooldown_timer <= 0.0f)
            {
                Vec2 bullet_vel = (player->entity()->pos - m_entity->pos).norm() * bullet_speed;
                Bullet::create(scene(), m_entity->pos, bullet_vel, 1.0f);

                m_shoot_cooldown_timer = shoot_cooldown;
            }

            // Appoach player
            auto mover = get<Mover>();
            const Vec2 pdir = (player->entity()->pos - m_entity->pos);
            mover->vel = Vec2::approach(mover->vel, pdir.norm() * 40.0f, 35.0f * elapsed);
        }
    }

    Entity* Enemy::create(Scene* scene, const Engine::Vec2& pos, const float radius)
    {
        Entity* e = scene->add_entity(pos);
        e->add(new Enemy());

        Collider* c = new CircleCollider(Circ(Vec2(), radius));
        c->mask = Mask::Enemy;
        e->add(c);

        c->visible = true;
        c->color = Color::white;

        Mover* m = new Mover();
        m->collider = c;
        m->stop_mask |= Mask::Enemy | Mask::Player | Mask::PlayerDash;
        e->add(m);

        return e;
    }
}
