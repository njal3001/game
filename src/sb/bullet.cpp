#include "sb/bullet.h"
#include "sb/scene.h"

namespace SB
{
    using namespace Engine;

    Bullet::Bullet(const Vec2& pos, const Vec2& vel, const float radius)
        : Entity(pos), vel(vel), m_collider(CircleCollider(radius))
    {}

    void Bullet::update(const float elapsed)
    {
        Player* player = m_scene->player;
        if (player && m_collider.Collider::intersects(pos, player->pos, *player->collider()))
        {
            printf("Hit by bullet!\n");
            destroy();
        }
        else
        {
            pos += vel * elapsed;
        }
    }

    void Bullet::render(Engine::Renderer* renderer)
    {
        renderer->circ(pos, m_collider.radius, 128, Color(0, 255, 255));
    }
}
