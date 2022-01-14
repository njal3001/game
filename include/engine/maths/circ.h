#pragma once
#include "engine/maths/vec2.h"
#include "engine/maths/line.h"

namespace Engine
{
    struct Circ
    {
        Vec2 center;
        float radius;

        Circ();
        Circ(const Vec2& center, const float radius);

        bool contains(const Vec2& point) const;
        bool intersects(const Line& line) const;

        bool intersects(const Circ& other) const;

        Circ offset(const Vec2& o) const;
    };
}
