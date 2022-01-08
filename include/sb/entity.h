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
        Scene* m_scene;

    public:
        Engine::Vec2 pos;

        Entity(const Engine::Vec2& pos);
        virtual ~Entity();

        void destroy();

        virtual void update(const float elapsed) = 0;
        virtual void render(Engine::Renderer* renderer) = 0;

    };
}
