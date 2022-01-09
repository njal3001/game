#pragma once
#include <unordered_set>
#include "engine/graphics/renderer.h"

namespace SB
{
    class Entity;
    class Player;

    class Scene
    {
    private:
        std::unordered_set<Entity*> m_entities;
        std::vector<Entity*> m_to_add;
        std::vector<Entity*> m_to_remove;

    public:
        // TODO: Implement map class
        Engine::Rect bounds;

        // TODO: Should propably not be public
        Player* player;

    public:
        Scene(const Engine::Rect& bounds);
        ~Scene();

        void add_entity(Entity* entity);
        void destroy_entity(Entity* entity);

        void update(float elapsed);
        void render(Engine::Renderer* renderer);

    private:
        void update_lists();
    };
}
