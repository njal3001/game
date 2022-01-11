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
        std::vector<Entity*> m_destroyed;
        std::vector<Entity*> m_to_add;
        std::vector<Entity*> m_to_remove;

        template <class T>
        struct LList
        {
            T* head = nullptr;
            T* tail = nullptr;

            void insert(T* val);
            void remove(T* val);
        };

        LList<Entity> m_entities[max_entity_types];

    public:
        // TODO: Implement map class
        Engine::Rect bounds;

    public:
        Scene(const Engine::Rect& bounds);
        ~Scene();

        template<class T>
        void add_entity(T* entity);
        void destroy_entity(Entity* entity);

        void update(float elapsed);
        void render(Engine::Renderer* renderer);

        template<class T>
        T* first() const;

    private:
        void update_lists();
    };

    template<class T>
    void Scene::LList<T>::insert(T* val)
    {
        if (head)
        {
            val->m_next = nullptr;
            val->m_prev = tail;
            tail->m_next = val;
            tail = val;
        }
        else
        {
            head = tail = val;
            val->m_next = val->m_prev = nullptr;
        }
    }
        
    template<class T>
    void Scene::LList<T>::remove(T* val)
    {
        if (val->m_prev)
        {
            val->m_prev->m_next = val->m_next;
        }
        if (val->m_next)
        {
            val->m_next->m_prev = val->m_prev;
        }

        if (val == head)
        {
            head = val->m_next;
        }
        if (val == tail)
        {
            tail = val->m_prev;
        }

        val->m_prev = val->m_next = nullptr;
    }

    // TODO: Look into perfect forwarding and T&&
    template<class T>
    void Scene::add_entity(T* entity)
    {
        assert(!entity->m_scene);

        m_to_add.push_back(entity);

        const uint8_t type = Entity::Types::id<T>();
        entity->m_scene = this;
        entity->m_type = type;
    }

    template<class T>
    T* Scene::first() const
    {
        const uint8_t type = Entity::Types::id<T>();
        const auto& ll = m_entities[type];

        return (T*)ll.head;
    }
}
