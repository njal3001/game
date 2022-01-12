#include "sb/ecs.h"

namespace SB
{
    Component::Component()
        : m_type(0), m_alive(true), m_entity(nullptr)
    {}

    Component::~Component()
    {}

    uint8_t Component::type() const
    {
        return m_type;
    }

    Entity* Component::entity() const
    {
        return m_entity;
    }

    Scene* Component::scene() const
    {
        return (m_entity ? m_entity->scene() : nullptr);
    }

    void Component::destroy()
    {
        m_alive = false;
    }

    void Component::awake()
    {}

    void Component::update(const float elapsed)
    {}

    void Component::render(Engine::Renderer* renderer)
    {}
}
