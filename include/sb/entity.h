#pragma once
#include "engine/maths/vec2.h"
#include "sb/collider.h"

namespace SB
{
    class Entity
    {
    public:
        Engine::Vec2 pos;
        Collider* const collider;

        Entity(const Engine::Vec2& pos, Collider* const collider);
    };
}
