#include "engine/maths/quad.h"

namespace Engine
{
    Quad::Quad()
        : a(), b(), c(), d()
    {}

    Quad::Quad(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d)
        : a(a), b(b), c(c), d(d)
    {}

    Quad::Quad(const Rect& rect)
        : a(rect.bottom_left()), b(rect.top_left()), c(rect.top_right()), d(rect.bottom_right())
    {}

    // TODO: Implement
    Quad::Quad(const Rect& rect, const float rotation)
    {

    }

    bool Quad::contains(const Vec2& point) const
    {
        return false;
    }

    bool Quad::intersects(const Line& line) const
    {
        return false;
    }

    bool Quad::intersects(const Quad& other) const
    {
        return false;
    }

    void Quad::rotate(const float delta)
    {

    }

    Quad Quad::offset(const Vec2& o) const
    {
        return Quad(a + o, b + o, c + o, d + o);
    }
}
