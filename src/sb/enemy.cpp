#include "sb/enemy.h"
#include "sb/bullet.h"
#include "sb/scene.h"

namespace SB
{
    using namespace Engine;

    Enemy::Enemy(const Vec2& pos, const float radius)
        : Entity(pos), m_collider(CircleCollider(radius)), m_shoot_cooldown_timer(0.0f)
    {}

    void Enemy::update(const float elapsed)
    {
        if (m_shoot_cooldown_timer <= 0.0f)
        {
            Vec2 bullet_vel = (m_scene->player->pos - pos).norm() * bullet_speed;
            Bullet* bullet = new Bullet(pos, bullet_vel, 1.0f);

            m_scene->add_entity(bullet);

            m_shoot_cooldown_timer = shoot_cooldown;
        }

        m_shoot_cooldown_timer -= elapsed;
    }

    void Enemy::render(Engine::Renderer* renderer)
    {
        Player* player = m_scene->player;

        Color c = Color::white;
        if (player && m_collider.Collider::intersects(pos, player->pos, *player->collider()))
        {
            c = Color(255, 255, 0);
        }

        renderer->circ(pos, m_collider.radius, 128, c);
    }
}
