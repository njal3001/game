#pragma once
#include <engine/maths/vec2.h>

namespace Engine
{
    struct Line
    {
        Vec2 start; 
        Vec2 end; 

        Line(const Vec2& start, const Vec2& end);

        float len() const;
        float len_squared() const;

        float distance(const Vec2& point) const;
    };
}
