#include "sb/entity.h"
#include "sb/scene.h"
#include <assert.h>

namespace SB
{
    Entity::Entity(const Engine::Vec2& pos)
        : m_scene(nullptr), pos(pos)
    {}

    Entity::~Entity()
    {}

    // Assumes heap allocation
    void Entity::destroy()
    {
        assert(m_scene);
        m_scene->destroy_entity(this);
    }

    void Entity::awake()
    {}

    void Entity::update(const float elapsed)
    {}

    void Entity::render(Engine::Renderer* renderer)
    {}
}
