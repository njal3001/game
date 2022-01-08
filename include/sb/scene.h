#pragma once
#include <unordered_set>
#include "engine/graphics/renderer.h"

namespace SB
{
    class Entity;

    class Scene
    {
    private:
        std::unordered_set<Entity*> m_entities;

    public:
        ~Scene();

        void add_entity(Entity* entity);
        void destroy_entity(Entity* entity);

        void update(float elapsed);
        void render(Engine::Renderer* renderer);
    };
}
