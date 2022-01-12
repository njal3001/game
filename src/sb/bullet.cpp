#include "sb/bullet.h"
#include "sb/boxcollider.h"

namespace SB
{
    using namespace Engine;

    Bullet::Bullet(const Vec2& vel, const float radius)
        : vel(vel), m_collider(CircleCollider(Circ(Vec2(), radius)))
    {}

    const Collider& Bullet::collider() const
    {
        return m_collider;
    }

    void Bullet::update(const float elapsed)
    {
        Player* player = scene()->first<Player>();
        if (player && m_collider.Collider::intersects(m_entity->pos, player->entity()->pos,
                    *player->collider()))
        {
            // Collided with player
            if (!player->dashing())
            {
                player->hurt();
                destroy();

                printf("Collided with player!\n");
            }
        }
        else
        {
            m_entity->pos += vel * elapsed;

            // Check bounds
            Rect bounds = scene()->bounds;
            BoxCollider scene_collider(Rect(0.0f, 0.0f, bounds.w, bounds.h));
            if (!m_collider.Collider::intersects(m_entity->pos, Vec2(bounds.center()),
                        scene_collider))
            {
                destroy();
            }
        }
    }

    void Bullet::render(Engine::Renderer* renderer)
    {
        renderer->circ(m_entity->pos, m_collider.bounds.radius, 128, Color(0, 255, 255));
    }
}
