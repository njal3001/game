#include "sb/enemy.h"
#include "sb/bullet.h"
#include "sb/boxcollider.h"

namespace SB
{
    using namespace Engine;

    Enemy::Enemy()
        : m_shoot_cooldown_timer(0.0f)
    {}

    void Enemy::update(const float elapsed)
    {
        if (m_shoot_cooldown_timer <= 0.0f)
        {
            Player* player = scene()->first<Player>();
            if (player)
            {
                /* Vec2 bullet_vel = (player->entity()->pos - m_entity->pos).norm() * bullet_speed; */
                /* Bullet::create(scene(), m_entity->pos, bullet_vel, 1.0f); */
            }

            m_shoot_cooldown_timer = shoot_cooldown;
        }
        else
        {
            m_shoot_cooldown_timer -= elapsed;
        }
    }

    void Enemy::render(Engine::Renderer* renderer)
    {
        /* const CircleCollider* collider = (CircleCollider*)get<Collider>(); */
        const BoxCollider* collider = (BoxCollider*)get<Collider>();
        Color c = Color::white;

        const Player* player = scene()->first<Player>();
        if (player)
        {
            const Collider* p_collider = player->get<Collider>();
            if (p_collider->intersects(*collider))
            {
                c = Color(255, 255, 0);
            }
        }

        //renderer->circ(m_entity->pos, collider->bounds.radius, 128, c);
        renderer->rect(collider->bounds.offset(m_entity->pos), c);
    }

    Entity* Enemy::create(Scene* scene, const Engine::Vec2& pos, const float radius)
    {
        Entity* e = scene->add_entity(pos);
        e->add(new Enemy());

        /* Collider* c = new CircleCollider(Circ(Vec2(), radius)); */
        Collider* c = new BoxCollider(Rect(0.0f, 0.0f, radius, radius));
        e->add(c);

        return e;
    }
}
