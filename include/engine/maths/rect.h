#pragma once
#include "engine/maths/shape.h"
#include "engine/maths/vec2.h"
#include "engine/maths/line.h"

namespace Engine
{
    struct Rect : Shape
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

        Line bottom() const;
        Line top() const;
        Line left() const;
        Line right() const;

        bool contains(const Vec2& point) const override;
        bool intersects(const Line& line) const override;

        bool intersects(const Rect& other) const;
    };
}
