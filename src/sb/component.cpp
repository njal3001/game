#include "sb/component.h"
#include "sb/entity.h"

namespace SB
{
    Component::Component(Entity* entity)
        : m_entity(entity), m_next(nullptr), m_prev(nullptr)
    {}

    uint8_t Component::type() const
    {
        return m_type;
    }

    Entity* Component::entity() const
    {
        return m_entity;
    }

    Component* Component::next() const
    {
        return m_next;
    }

    Component* Component::prev() const
    {
        return m_prev;
    }

    void destroy()
    {
        // TODO: Call scene function
    }
}
