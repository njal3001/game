#pragma once
#include "engine/maths/vec2.h"
#include "engine/maths/line.h"

namespace Engine
{
    struct Shape
    {
        virtual bool contains(const Vec2& point) const = 0;
        virtual bool intersects(const Line& line) const = 0;
    };
}
