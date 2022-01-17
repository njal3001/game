#pragma once
#include "engine/graphics/renderer.h"
#include "sb/collider.h"
#include "engine/maths/rect.h"

namespace SB
{
    class BoxCollider : public Collider
    {
    public:
        Engine::Rect bounds;
        float rotation; // In radians

    public:
        BoxCollider(const Engine::Rect& bounds);
        BoxCollider(const Engine::Rect& bounds, const float rotation);

        Engine::Vec2 offset() const override;

        void render(Engine::Renderer* renderer) override;

    protected:
        std::vector<Engine::Vec2> axes(const std::vector<Engine::Vec2>& other_vertices) const override;
        std::vector<Engine::Vec2> vertices() const override;

        std::function<std::vector<Engine::Vec2> (const Engine::Vec2& vertex, 
                const Engine::Vec2& axis)>
            vertex_mapper() const override;
    };
}
