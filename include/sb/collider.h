#pragma once
#include <vector>
#include "engine/maths/shape.h"
#include "engine/maths/vec2.h"
#include "engine/maths/line.h"

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

    Engine::Shape* m_shape;

    protected:
        virtual std::vector<Engine::Vec2> get_axes(const Collider& other) const = 0;
        virtual Projection get_projection(const Engine::Vec2& axis) const = 0;

    public:
        virtual Engine::Vec2 center() const = 0;
        virtual void move(const Engine::Vec2& amount) = 0;

        bool contains(const Engine::Vec2& point) const;
        bool intersects(const Engine::Line& line) const;
        bool intersects(const Collider& other) const;
        
        // Returns a vector that pushes this collider out of the other collider
        Engine::Vec2 static_displacement(const Collider& other) const;

    };
}
