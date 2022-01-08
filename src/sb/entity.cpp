#include "sb/entity.h"
#include <assert.h>
#include "sb/scene.h"

namespace SB
{
    Entity::Entity(const Engine::Vec2& pos)
        : m_scene(nullptr), pos(pos)
    {}

    // Assumes heap alloaction
    void Entity::destroy()
    {
        assert(m_scene);
        m_scene->destroy_entity(this);
    }

    Entity::~Entity()
    {}
}
