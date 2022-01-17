#pragma once
#include "sb/ecs.h"
#include "sb/player.h"
#include "sb/circlecollider.h"

namespace SB
{
    class Bullet : public Component
    {
    public:
        static Entity* create(Scene* scene, const Engine::Vec2& pos,
                const Engine::Vec2& vel, const float radius);
    };
}
