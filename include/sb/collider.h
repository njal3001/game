#pragma once
#include <vector>
#include <functional>
#include "engine/maths/vec2.h"
#include "engine/maths/line.h"
#include "engine/graphics/renderer.h"
#include "sb/ecs.h"

namespace SB
{
    class Collider : public Component
    {
    protected:
        struct Projection
        {
            float start;
            float end;
        };

        static const std::vector<Engine::Vec2> rect_axes;

    protected:
        virtual std::vector<Engine::Vec2> axes(const Collider& other) const = 0;
        virtual std::vector<Engine::Vec2> vertices() const = 0;
        //virtual Projection projection(const Engine::Vec2& axis) const = 0;

        // Some collider projections are axis dependent and use a mapper
        virtual std::function<std::vector<Engine::Vec2> (const Engine::Vec2& vertex, 
                const Engine::Vec2& axis)> vertex_mapper() const = 0;

        static Engine::Vec2 circ_axis(const Engine::Circ& circ, const Collider& other);
        static std::vector<Engine::Vec2> rect_vertices(const Engine::Rect& rect);

    public:
        // Colliders offset from entity position
        virtual Engine::Vec2 offset() const = 0;

        // TODO: Implement wit SAT
        virtual bool contains(const Engine::Vec2& point) const = 0;
        virtual bool intersects(const Engine::Line& line) const = 0;

        bool intersects(const Collider& other) const;
        
        // Returns a vector that pushes this collider out of the other collider
        Engine::Vec2 displace(const Collider& other) const;

        bool intersects(const Engine::Rect& rect) const;
        bool intersects(const Engine::Circ& circ) const;

    private:
        static Projection projection(const std::vector<Engine::Vec2>& vertices, 
                const Engine::Vec2& axis, 
                const std::function<std::vector<Engine::Vec2> 
                (const Engine::Vec2& vertex, const Engine::Vec2& axis)> mapper);

        static bool intersects(const std::vector<Engine::Vec2>& v1, 
                const std::vector<Engine::Vec2>& v2,
                const std::vector<Engine::Vec2>& a1,
                const std::vector<Engine::Vec2>& a2,
                const std::function<std::vector<Engine::Vec2> 
                (const Engine::Vec2& v, const Engine::Vec2& a)> m1,
                const std::function<std::vector<Engine::Vec2> 
                (const Engine::Vec2& v, const Engine::Vec2& a)> m2);

        static Engine::Vec2 displace(const std::vector<Engine::Vec2>& v1, 
                const std::vector<Engine::Vec2>& v2,
                const std::vector<Engine::Vec2>& a1,
                const std::vector<Engine::Vec2>& a2,
                const std::function<std::vector<Engine::Vec2> 
                (const Engine::Vec2& v, const Engine::Vec2& a)> m1,
                const std::function<std::vector<Engine::Vec2> 
                (const Engine::Vec2& v, const Engine::Vec2& a)> m2);
    };
}
