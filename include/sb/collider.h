#pragma once
#include <vector>
#include "engine/maths/shape.h"
#include "engine/maths/vec2.h"
#include "engine/maths/line.h"
#include "engine/graphics/renderer.h"

namespace SB
{
    class Collider
    {
    protected:
        struct Projection
        {
            float start;
            float end;
        };

    protected:
        virtual std::vector<Engine::Vec2> get_axes(const Engine::Vec2& pos, 
                const Engine::Vec2& pos_other) const = 0;
        virtual Projection get_projection(const Engine::Vec2& pos, const Engine::Vec2& axis) const = 0;

    public:
        // TODO: Could get shape from subclasses instead and implement in this class
        virtual bool contains(const Engine::Vec2& pos, const Engine::Vec2& point) const = 0;
        virtual bool intersects(const Engine::Vec2& pos, const Engine::Line& line) const = 0;

        bool intersects(const Engine::Vec2& pos, const Engine::Vec2& pos_other, const Collider& other) const;
        
        // Returns a vector that pushes this collider out of the other collider
        Engine::Vec2 static_displacement(const Engine::Vec2& pos, const Engine::Vec2& pos_other, const Collider& other) const;

        virtual void render(const Engine::Vec2& pos, Engine::Renderer* renderer) const = 0;
    };
}
