#pragma once
#include "engine/maths/vec2.h"

namespace Engine
{
    struct Rect
    {
        float x, y;
        float w, h;

        Rect();
        Rect(float x, float y, float w, float h);
        Rect(const Vec2& pos, const Vec2& size);

        Vec2 center() const;
        Vec2 bottom_left() const;
        Vec2 top_left() const;
        Vec2 top_right() const;
        Vec2 bottom_right() const;

        bool contains(const Vec2& point) const;
        bool overlaps(const Rect& other) const;
    };
}
