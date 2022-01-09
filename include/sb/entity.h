#pragma once
#include "engine/maths/vec2.h"
#include "engine/graphics/renderer.h"

namespace SB
{
    class Scene;

    class Entity
    {
        friend class Scene;

    private:
        uint8_t m_type;

        class Types
        {
            private:
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
        Scene* m_scene;

    public:
        Engine::Vec2 pos;

        Entity(const Engine::Vec2& pos);
        virtual ~Entity();

        uint8_t type() const;

        // Called after being added to scene
        virtual void awake();

        virtual void destroy();

        virtual void update(const float elapsed);
        virtual void render(Engine::Renderer* renderer);
    };
}
