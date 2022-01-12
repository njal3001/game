#include "sb/ecs.h"
#include <algorithm>

namespace SB
{
    Entity::Entity(const Engine::Vec2& pos)
        : pos(pos), m_scene(nullptr), m_alive(true)
    {}

    Entity::~Entity()
    {
        for (auto c : m_to_add)
        {
            delete c;
        }

        for (auto c : m_components)
        {
            delete c;
        }
    }

    Scene* Entity::scene() const
    {
        return m_scene;
    }

    void Entity::destroy()
    {
        m_alive = false;

        for (auto c : m_components)
        {
            c->m_alive = false;
        }
    }

    void Entity::update_lists()
    {
        auto it = m_components.begin();

        while (it != m_components.end())
        {
            Component* c = *it;
            if (!c->m_alive)
            {
                it = m_components.erase(it);

                m_scene->untrack_component(c);
                delete c;
            }
            else
            {
                it++;
            }
        }

        for (auto c : m_to_add)
        {
            c->m_entity = this;
            m_components.push_back(c);
            m_scene->track_component(c);

            c->awake();
        }

        m_to_add.clear();
    }
}
