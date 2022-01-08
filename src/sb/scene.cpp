#include "sb/scene.h"
#include <algorithm>
#include "engine/log.h"
#include "sb/entity.h"
#include <assert.h>

namespace SB
{
    using namespace Engine;

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
    }

    void Scene::destroy_entity(Entity* entity)
    {
        assert(entity->m_scene == this);

        auto pos = std::find(m_entities.begin(), m_entities.end(), entity);
        m_entities.erase(pos);

        delete entity;
    }

    void Scene::update(float elapsed)
    {
        for (auto e : m_entities)
        {
            e->update(elapsed);
        }
    }

    void Scene::render(Engine::Renderer* renderer)
    {
        for (auto e : m_entities)
        {
            e->render(renderer);
        }
    }
}
