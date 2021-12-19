#include "sb/collision.h"
#include "engine/maths/calc.h"

namespace SB
{
    using namespace Engine;

    Vec2 Collision::rect_rect(const Rect& r1, const Rect& r2)
    {
        if (!r1.overlaps(r2))
        {
            return Vec2();
        }

        Vec2 right_push = Vec2(r1.x + r1.w - r2.x, 0.0f);
        Vec2 left_push = Vec2(r1.x - (r2.x + r2.w), 0.0f);
        Vec2 up_push = Vec2(0.0f, r1.y + r1.h - r2.y);
        Vec2 down_push = Vec2(0.0f, r1.y - (r2.y + r2.h));

        Vec2 min_x = right_push.x < Calc::abs(left_push.x) ? right_push : left_push;
        Vec2 min_y = up_push.y < Calc::abs(down_push.y) ? up_push : down_push;

        return Calc::abs(min_x.x) < Calc::abs(min_y.y) ? min_x : min_y; 
    }
}
