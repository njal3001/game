#include "sb/bullet.h"
#include "sb/scene.h"
#include "sb/boxcollider.h"

namespace SB
{
    using namespace Engine;

    Bullet::Bullet(const Vec2& pos, const Vec2& vel, const float radius)
        : Entity(pos), vel(vel), m_collider(CircleCollider(radius))
    {}

    void Bullet::update(const float elapsed)
    {
        Player* player = m_scene->first<Player>();
        if (player && m_collider.Collider::intersects(pos, player->pos, *player->collider()))
        {
            // Collided with player
            player->hurt();
            destroy();
        }
        else
        {
            pos += vel * elapsed;

            // Check bounds
            Rect bounds = m_scene->bounds;
            BoxCollider scene_collider(Vec2(bounds.w, bounds.h));
            if (!m_collider.Collider::intersects(pos, Vec2(bounds.center()), scene_collider))
            {
                destroy();
            }
        }
    }

    void Bullet::render(Engine::Renderer* renderer)
    {
        renderer->circ(pos, m_collider.radius, 128, Color(0, 255, 255));
    }
}
