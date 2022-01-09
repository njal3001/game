#pragma once
#include "engine/maths/vec2.h"
#include "engine/graphics/renderer.h"

namespace SB
{
    class Scene;

    class Entity
    {
        friend class Scene;

    protected:
        Scene* m_scene;

    public:
        Engine::Vec2 pos;

        Entity(const Engine::Vec2& pos);
        virtual ~Entity();

        // Called after being added to scene
        virtual void awake();

        virtual void destroy();

        virtual void update(const float elapsed);
        virtual void render(Engine::Renderer* renderer);
    };
}
