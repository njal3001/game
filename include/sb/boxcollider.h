#pragma once
#include "sb/collider.h"
#include "engine/maths/rect.h"

namespace SB
{
    class BoxCollider : public Collider
    {
    public:
        Engine::Rect bounds;

    public:
        BoxCollider(const Engine::Rect& bounds);

        Engine::Vec2 offset() const override;

        bool contains(const Engine::Vec2& point) const override;
        bool intersects(const Engine::Line& line) const override;

    protected:
        std::vector<Engine::Vec2> axes(const std::vector<Engine::Vec2>& other_vertices) const override;
        std::vector<Engine::Vec2> vertices() const override;

        std::function<std::vector<Engine::Vec2> (const Engine::Vec2& vertex, 
                const Engine::Vec2& axis)>
            vertex_mapper() const override;
    };
}
