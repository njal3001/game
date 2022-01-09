#include "sb/scene.h"
#include <algorithm>
#include "engine/log.h"
#include "sb/entity.h"
#include <assert.h>

namespace SB
{
    using namespace Engine;

    Scene::Scene(const Rect& bounds)
        : bounds(bounds), player(nullptr)
    {}

    Scene::~Scene()
    {
        for (auto e : m_entities)
        {
            delete e;
        }
    }

    void Scene::add_entity(Entity* entity)
    {
        assert(!entity->m_scene);

        bool added = m_entities.insert(entity).second;
        assert(added);

        entity->m_scene = this;
        entity->awake();
    }

    void Scene::destroy_entity(Entity* entity)
    {
        assert(entity->m_scene == this);
        m_to_remove.push_back(entity);
    }

    void Scene::update(float elapsed)
    {
        update_lists();

        for (auto e : m_entities)
        {
            e->update(elapsed);
        }
    }

    void Scene::update_lists()
    {
        for (auto entity : m_to_remove)
        {
            auto pos = std::find(m_entities.begin(), m_entities.end(), entity);
            m_entities.erase(pos);

            delete entity;
        }

        m_to_remove.clear();

        for (auto entity : m_to_add)
        {
            bool added = m_entities.insert(entity).second;
            assert(added);

            entity->m_scene = this;
            entity->awake();
        }

        m_to_add.clear();
    }

    void Scene::render(Engine::Renderer* renderer)
    {
        for (auto e : m_entities)
        {
            e->render(renderer);
        }
    }
}
