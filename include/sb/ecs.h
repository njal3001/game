#pragma once
#include <assert.h>
#include "engine/graphics/renderer.h"

namespace SB
{
    class Entity;
    class Scene;

    class Component
    {
        friend class Scene;
        friend class Entity;

    private:
        uint8_t m_type;
        bool m_alive;

        class Types
        {
            inline static uint8_t s_counter = 0;

        public:
            template<class T>
            static uint8_t id()
            {
                static const uint8_t val = s_counter++;
                return val;
            }
        };

    protected:
        Entity* m_entity;

    public:
        Component();
        virtual ~Component();

        uint8_t type() const;

        Entity* entity() const;
        Scene* scene() const;

        virtual void awake();
        virtual void destroy();

        virtual void update(const float elapsed);
        virtual void render(Engine::Renderer* renderer);
    };

    class Entity
    {
        friend class Scene;

    public:
        Engine::Vec2 pos;

    private:
        Scene* m_scene;
        // TODO: Use unique ptr
        std::vector<Component*> m_components;
        std::vector<Component*> m_to_add;

        bool m_alive;

    public:
        Entity(const Engine::Vec2& pos);
        ~Entity();

        Scene* scene() const;

        template <class T>
        void add(T* component);

        template <class T>
        T* get() const;

        void remove(Component* component);

        void destroy();

    private:
        void removed();
        void update_lists();
    };

    class Scene
    {
    public:
        static constexpr int max_component_types = 256;

    private:
        // TODO: Use unique ptr
        std::vector<Entity*> m_entities;
        std::vector<Entity*> m_to_add;
        std::vector<Component*> m_components[max_component_types];

    public:
        // TODO: Implement map class
        Engine::Rect bounds;

    public:
        Scene(const Engine::Rect& bounds);
        ~Scene();

        Entity* add_entity(const Engine::Vec2& pos);
        void remove_entity(Entity* entity);

        // TODO: Let entity be friend and change to private?
        void track_component(Component* component);
        void untrack_component(Component* component);

        void update(float elapsed);
        void render(Engine::Renderer* renderer);

        template<class T>
        T* first() const;

        template<class T>
        std::vector<T*> all() const;

    private:
        void update_lists();
    };

    template <class T>
    void Entity::add(T* component)
    {
        assert(m_scene);

        const uint8_t type = Component::Types::id<T>();
        component->m_type = type;

        m_to_add.push_back(component);
    }

    template <class T>
    T* Entity::get() const
    {
        const uint8_t type = Component::Types::id<T>();
        for (auto c : m_components)
        {
            if (c->m_type == type)
            {
                return c;
            }
        }

        return nullptr;
    }

    template<class T>
    T* Scene::first() const
    {
        const uint8_t type = Component::Types::id<T>();
        auto& c_vec = m_components[type];

        auto it = c_vec.begin();

        while (it != c_vec.end())
        {
            auto c = *it;
            if (c->m_alive)
            {
                return (T*)c;
            }
        }

        return nullptr;
    }

    template<class T>
    std::vector<T*> Scene::all() const
    {
        const uint8_t type = Component::Types::id<T>();
        auto& c_vec = m_components[type];

        // TODO: Slow
        std::vector<T*> v_all;
        for (auto c : c_vec)
        {
            if (c->m_alive)
            {
                v_all.push_back((T*)c);
            }
        }

        return v_all;
    }
}
