#include "sb/entity.h"

namespace SB
{
    Entity::Entity(const Engine::Vec2& pos, Collider* const collider)
        : pos(pos), collider(collider)
    {}
}
