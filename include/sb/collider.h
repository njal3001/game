#pragma once
#include <vector>
#include <functional>
#include "engine/maths/vec2.h"
#include "engine/maths/line.h"
#include "engine/graphics/renderer.h"
#include "sb/ecs.h"

namespace SB
{
    struct Mask
    {
        static constexpr uint32_t None = 0;
        static constexpr uint32_t Solid = 1;
        static constexpr uint32_t Enemy = 1 << 1;
        static constexpr uint32_t Player = 1 << 2;
        static constexpr uint32_t PlayerDash = 1 << 3;
    };

    // TODO: This turned into an overcomplicated mess...
    // Should redo it all at some point
    class Collider : public Component
    {
    public:
        uint32_t mask;

    private:
        struct Projection
        {
            float start;
            float end;
        };

    protected:
        static const std::vector<Engine::Vec2> rect_axes;

    protected:
        virtual std::vector<Engine::Vec2> axes(const std::vector<Engine::Vec2>& other_vertices) const = 0;
        virtual std::vector<Engine::Vec2> vertices() const = 0;

        // Some collider projections are axis dependent and use a mapper
        virtual std::function<std::vector<Engine::Vec2> (const Engine::Vec2& vertex, 
                const Engine::Vec2& axis)> vertex_mapper() const = 0;

        static Engine::Vec2 circ_axis(const Engine::Circ& circ, const std::vector<Engine::Vec2>& other_vertices);
        static std::vector<Engine::Vec2> rect_vertices(const Engine::Rect& rect);

    public:
        Collider();
        // Colliders offset from entity position
        virtual Engine::Vec2 offset() const = 0;

        // TODO: Implement wit SAT
        virtual bool contains(const Engine::Vec2& point) const = 0;
        virtual bool intersects(const Engine::Line& line) const = 0;

        bool intersects(const Collider& other) const;
        
        // Returns a vector that pushes this collider out of the other collider
        Engine::Vec2 displace(const Collider& other) const;

        bool intersects(const Engine::Rect& rect) const;

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
