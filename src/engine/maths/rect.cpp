#include "engine/maths/rect.h"
#include "engine/maths/collision.h"

namespace Engine
{
    Rect::Rect()
        : Rect(0.0f, 0.0f, 0.0f, 0.0f)
    {}

    Rect::Rect(float x, float y, float w, float h)
        : x(x), y(y), w(w), h(h)
    {}

    Rect::Rect(const Vec2& pos, const Vec2& size)
        : Rect(pos.y, pos.y, size.x, size.y)
    {}

    Vec2 Rect::center() const
    {
        return Vec2(x + (w/2.0f), y + (h/2.0f));
    }

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

    Line Rect::bottom() const
    {
        return Line(bottom_left(), bottom_right());
    }

    Line Rect::top() const
    {
        return Line(top_left(), top_right());
    }

    Line Rect::left() const
    {
        return Line(bottom_left(), top_left());
    }

    Line Rect::right() const
    {
        return Line(bottom_right(), top_right());
    }

    bool Rect::contains(const Vec2& point) const
    {
        return 
            x <= point.x && point.x <= x + w && 
            y <= point.y && point.y <= y + h;
    }

    bool Rect::intersects(const Line& line) const
    {
        return false;
    }

    bool Rect::intersects(const Rect& other) const
    {
        return 
            x <= other.x + other.w &&
            x + h >= other.x &&
            y <= other.y + other.h &&
            y + h >= other.y;
    }
}
