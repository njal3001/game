#include "sb/bullet.h"
#include "sb/scene.h"
#include "sb/boxcollider.h"

namespace SB
{
    using namespace Engine;

    Bullet::Bullet(const Vec2& pos, const Vec2& vel, const float radius)
        : Entity(pos), vel(vel), m_collider(CircleCollider(Circ(Vec2(), radius)))
    {}

    const Collider& Bullet::collider() const
    {
        return m_collider;
    }

    void Bullet::update(const float elapsed)
    {
        Player* player = m_scene->first<Player>();
        if (player && m_collider.Collider::intersects(pos, player->pos, *player->collider()))
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
            pos += vel * elapsed;

            // Check bounds
            Rect bounds = m_scene->bounds;
            BoxCollider scene_collider(Rect(0.0f, 0.0f, bounds.w, bounds.h));
            if (!m_collider.Collider::intersects(pos, Vec2(bounds.center()), scene_collider))
            {
                destroy();
            }
        }
    }

    void Bullet::render(Engine::Renderer* renderer)
    {
        renderer->circ(pos, m_collider.bounds.radius, 128, Color(0, 255, 255));
    }
}
