#include "sb/enemy.h"
#include "sb/bullet.h"

namespace SB
{
    using namespace Engine;

    Enemy::Enemy(const float radius)
        : m_collider(CircleCollider(Circ(Vec2(), radius))), 
        m_shoot_cooldown_timer(0.0f)
    {}

    void Enemy::update(const float elapsed)
    {
        if (m_shoot_cooldown_timer <= 0.0f)
        {
            Player* player = scene()->first<Player>();
            if (player)
            {
                Entity* b_entity = scene()->add_entity(m_entity->pos);

                Vec2 bullet_vel = (player->entity()->pos - m_entity->pos).norm() * bullet_speed;
                Bullet* bullet = new Bullet(bullet_vel, 1.0f);
                b_entity->add(bullet);

                m_shoot_cooldown_timer = shoot_cooldown;
            }
        }

        m_shoot_cooldown_timer -= elapsed;
    }

    void Enemy::render(Engine::Renderer* renderer)
    {
        Player* player = scene()->first<Player>();

        Color c = Color::white;
        if (player && m_collider.Collider::intersects(m_entity->pos, player->entity()->pos, 
                    *player->collider()))
        {
            c = Color(255, 255, 0);
        }

        renderer->circ(m_entity->pos, m_collider.bounds.radius, 128, c);
    }
}
