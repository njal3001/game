#include "sb/enemy.h"

namespace SB
{
    using namespace Engine;

    Enemy::Enemy(const Vec2& pos, const float radius, Player* player)
        : Entity(pos), m_collider(CircleCollider(radius)), m_player(player)
    {}

    void Enemy::update(const float elapsed)
    {}

    void Enemy::render(Engine::Renderer* renderer)
    {
        Color c = Color::white;
        if (m_collider.Collider::intersects(pos, m_player->pos, *m_player->collider()))
        {
            c = Color(255, 255, 0);
        }

        renderer->circ(pos, m_collider.radius, 128, c);
    }
}
