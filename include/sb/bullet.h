#pragma once
#include "sb/ecs.h"
#include "sb/player.h"
#include "sb/circlecollider.h"

namespace SB
{
    class Bullet : public Component
    {
    private:
        float m_lifetime;

    public:
        Bullet(const float lifetime);

        void update(const float elapsed) override;

        static Entity* create(Scene* scene, const Engine::Vec2& pos,
                const Engine::Vec2& vel, const float lifetime, const float radius);
    };
}
