#pragma once
#include "sb/collider.h"
#include "engine/maths/circ.h"

namespace SB
{
    class CircleCollider : public Collider
    {
    public:
        Engine::Circ bounds;

    public:
        CircleCollider(const Engine::Circ& bounds);

        Engine::Vec2 offset() const override;

        void render(Engine::Renderer* renderer) override;

    protected:
        std::vector<Engine::Vec2> axes(const std::vector<Engine::Vec2>& other_vertices) const override;
        std::vector<Engine::Vec2> vertices() const override;

        std::function<std::vector<Engine::Vec2> (const Engine::Vec2& vertex,
                const Engine::Vec2& axis)> vertex_mapper() const override;
    };
}
