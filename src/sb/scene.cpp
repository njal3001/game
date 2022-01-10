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
        for (auto& e_vec : m_entities)
        {
            for (auto e : e_vec)
            {
                delete e;
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

        for (auto& e_vec : m_entities)
        {
            for (auto e : e_vec)
            {
                if (e->m_scene == this)
                {
                    e->update(elapsed);
                }
            }
        }
    }

    void Scene::update_lists()
    {
        for (auto entity : m_to_remove)
        {
            auto& entity_vec = m_entities[entity->m_type];
            auto pos = std::find(entity_vec.begin(), entity_vec.end(), entity);
            entity_vec.erase(pos);

            entity->m_scene = nullptr;
            m_destroyed.push_back(entity);
        }

        m_to_remove.clear();

        for (auto entity : m_to_add)
        {
            auto& entity_vec = m_entities[entity->m_type];
            entity_vec.push_back(entity);

            entity->awake();
        }

        m_to_add.clear();
    }

    void Scene::render(Engine::Renderer* renderer)
    {
        for (auto& e_vec : m_entities)
        {
            for (auto e : e_vec)
            {
                if (e->m_scene == this)
                {
                    e->render(renderer);
                }
            }
        }
    }
}
