#pragma once
#include "sb/entity.h"
#include "sb/player.h"
#include "sb/circlecollider.h"

namespace SB
{
    class Bullet : public Entity
    {
    public:
        Engine::Vec2 vel;

    private:
        CircleCollider m_collider;
        Player* m_player;

    public:
        Bullet(const Engine::Vec2& pos, const Engine::Vec2& vel, const float radius);

        void update(const float elapsed) override;
        void render(Engine::Renderer* renderer) override;
    };
}
