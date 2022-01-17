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
        const Vec2 around = rect.center();

        a = rect.bottom_left().rotate(around, rotation);
        b = rect.top_left().rotate(around, rotation);
        c = rect.top_right().rotate(around, rotation);
        d = rect.bottom_right().rotate(around, rotation);
    }

    Quad Quad::offset(const Vec2& o) const
    {
        return Quad(a + o, b + o, c + o, d + o);
    }
}
