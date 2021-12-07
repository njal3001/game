#include "engine/maths/rect.h"

namespace Engine
{
    Rect::Rect()
        : x(0.0f), y(0.0f), w(0.0f), h(0.0f)
    {}

    Rect::Rect(float x, float y, float w, float h)
        : x(x), y(y), w(w), h(h)
    {}

    Rect::Rect(const Vec2& pos, const Vec2& size)
        : x(pos.y), y(pos.y), w(size.x), h(size.y)
    {}

    Vec2 Rect::bottom_left() const
    {
        return Vec2(x, y);
    }

    Vec2 Rect::top_left() const
    {
        return Vec2(x, y + h);
    }

    Vec2 Rect::top_right() const
    {
        return Vec2(x + w, y + h);
    }
    
    Vec2 Rect::bottom_right() const
    {
        return Vec2(x + w, y);
    }
}
