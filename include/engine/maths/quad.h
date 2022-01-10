#include "engine/maths/vec2.h"
#include "engine/maths/rect.h"

namespace Engine
{
    struct Quad
    {
        Vec2 a;
        Vec2 b;
        Vec2 c;
        Vec2 d;

        Quad();
        Quad(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d);
        Quad(const Rect& rect);
        Quad(const Rect& rect, const float rotation);

        bool contains(const Vec2& point) const;
        bool intersects(const Line& line) const;

        bool intersects(const Quad& other) const;

        void rotate(const float delta);

        Quad offset(const Vec2& o) const;
    };
}
