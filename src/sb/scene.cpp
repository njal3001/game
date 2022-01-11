#include "sb/scene.h"
#include <algorithm>
#include "engine/log.h"
#include "sb/entity.h"
#include <assert.h>

namespace SB
{
    using namespace Engine;

    Scene::Scene(const Rect& bounds)
        : bounds(bounds)
    {}

    Scene::~Scene()
    {
        for (auto& ll : m_entities)
        {
            Entity* e = ll.head;
            while (e)
            {
                Entity* next = e->m_next;

                delete e;
                e = next;
            }
        }

        for (auto e : m_destroyed)
        {
            delete e;
        }
    }

    void Scene::destroy_entity(Entity* entity)
    {
        assert(entity->m_scene == this);
        entity->m_scene = nullptr;
        m_to_remove.push_back(entity);
    }

    void Scene::update(float elapsed)
    {
        update_lists();

        for (auto& ll : m_entities)
        {
            Entity* e = ll.head;
            while (e)
            {
                if (e->m_scene == this)
                {
                    e->update(elapsed);
                }

                e = e->m_next;
            }
        }
    }

    void Scene::update_lists()
    {
        for (auto entity : m_to_remove)
        {
            auto& ll = m_entities[entity->m_type];
            ll.remove(entity);

            entity->m_scene = nullptr;
            m_destroyed.push_back(entity);
        }

        m_to_remove.clear();

        for (auto entity : m_to_add)
        {
            auto& ll = m_entities[entity->m_type];
            ll.insert(entity);

            entity->awake();
        }

        m_to_add.clear();
    }

    void Scene::render(Engine::Renderer* renderer)
    {
        for (const auto& ll : m_entities)
        {
            Entity* e = ll.head;

            while (e)
            {
                if (e->m_scene == this)
                {
                    e->render(renderer);
                }

                e = e->m_next;
            }
        }
    }
}
