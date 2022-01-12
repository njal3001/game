#include "sb/ecs.h"
#include <algorithm>

namespace SB
{
    using namespace Engine;

    Scene::Scene(const Rect& bounds)
        : bounds(bounds)
    {}

    Scene::~Scene()
    {
        for (auto e : m_to_add)
        {
            delete e;
        }

        for (auto e : m_entities)
        {
            delete e;
        }
    }

    Entity* Scene::add_entity(const Engine::Vec2& pos)
    {
        Entity* entity = new Entity(pos);
        entity->m_scene = this;

        m_to_add.push_back(entity);

        return entity;
    }

    void Scene::track_component(Component* component)
    {
        m_components[component->type()].push_back(component);
    }

    void Scene::untrack_component(Component* component)
    {
        assert(component->scene() == this);

        auto& c_vec = m_components[component->m_type];
        c_vec.erase(std::find(std::begin(c_vec), std::end(c_vec), component));
    }

    void Scene::update(float elapsed)
    {
        update_lists();

        for (auto e : m_entities)
        {
            e->update_lists();
        }

        for (auto& c_vec : m_components)
        {
            for (auto c : c_vec)
            {
                if (c->scene() == this)
                {
                    c->update(elapsed);
                }
            }
        }
    }

    void Scene::update_lists()
    {
        auto it = m_entities.begin();

        while (it != m_entities.end())
        {
            if (!(*it)->m_alive)
            {
                auto next = m_entities.erase(it);
                delete *it;

                it = next;
            }
            else
            {
                it++;
            }
        }

        for (auto entity : m_to_add)
        {
            m_entities.push_back(entity);
        }

        m_to_add.clear();
    }

    void Scene::render(Engine::Renderer* renderer)
    {
        for (const auto& c_vec : m_components)
        {
            for (auto c : c_vec)
            {
                if (c->m_alive)
                {
                    c->render(renderer);
                }
            }
        }
    }
}
