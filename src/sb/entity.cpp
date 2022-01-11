#include "sb/entity.h"
#include "sb/scene.h"
#include <assert.h>

namespace SB
{
    Entity::Entity(const Engine::Vec2& pos)
        : pos(pos), m_type(0), m_scene(nullptr), m_next(nullptr), m_prev(nullptr)
    {}

    Entity::~Entity()
    {}

    uint8_t Entity::type() const
    {
        return m_type;
    }
    
    Entity* Entity::next() const
    {
        return m_next;
    }

    Entity* Entity::prev() const
    {
        return m_prev;
    }

    // Assumes heap allocation
    void Entity::destroy()
    {
        if (m_scene)
        {
            m_scene->destroy_entity(this);
        }
    }

    void Entity::awake()
    {}

    void Entity::update(const float elapsed)
    {}

    void Entity::render(Engine::Renderer* renderer)
    {}
}
