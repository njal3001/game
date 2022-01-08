#pragma once
#include "sb/entity.h"
#include "sb/player.h"
#include "sb/circlecollider.h"

namespace SB
{
    class Enemy : public Entity
    {
    private:
        CircleCollider m_collider;
        Player* m_player;

    public:
        Enemy(const Engine::Vec2& pos, const float radius, Player* player);

        void update(const float elapsed) override;
        void render(Engine::Renderer* renderer) override;
    };
}
