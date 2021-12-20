#pragma once
#include "engine/maths/shape.h"
#include "engine/maths/vec2.h"
#include "engine/maths/line.h"

namespace Engine
{
    struct Circ : Shape
    {
        Vec2 center;
        float radius;

        Circ();
        Circ(const Vec2& center, const float radius);

        bool contains(const Vec2& point) const override;
        bool intersects(const Line& line) const override;

        bool intersects(const Circ& other) const;
    };
}
