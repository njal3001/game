#pragma once
#include <assert.h>
#include "entity.h"
#include "engine/graphics/renderer.h"

namespace SB
{
    class Player;

    class Scene
    {
    public:
        static constexpr int max_entity_types = 256;

    private:
        // TODO: Change to linked list for faster remove and subclass casting
        std::vector<Entity*> m_entities[max_entity_types];

        std::vector<Entity*> m_destroyed;
        std::vector<Entity*> m_to_add;
        std::vector<Entity*> m_to_remove;


    public:
        // TODO: Implement map class
        Engine::Rect bounds;

    public:
        Scene(const Engine::Rect& bounds);
        ~Scene();

        // TODO: Look into perfect forwarding and T&&
        template<class T>
        void add_entity(T* entity)
        {
            assert(!entity->m_scene);

            m_to_add.push_back(entity);

            const uint8_t type = Entity::Types::id<T>();
            entity->m_scene = this;
            entity->m_type = type;
        }

        void destroy_entity(Entity* entity);

        void update(float elapsed);
        void render(Engine::Renderer* renderer);

        template<class T>
        T* first() const
        {
            const uint8_t type = Entity::Types::id<T>();
            const auto& e_vec = m_entities[type];

            if (e_vec.size() > 0)
            {
                return (T*)e_vec[0];
            }

            return nullptr;
        }

        template<class T>
        std::vector<T*> all() const
        {
            const uint8_t type = Entity::Types::id<T>();
            const auto&  e_vec = m_entities[type];

            // TODO: Slow, but I want the cast
            std::vector<T*> res;
            for (auto e : e_vec)
            {
                res.push_back((T*)e);
            }

            return res;
        }

    private:
        void update_lists();
    };
}
