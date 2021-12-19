#include "engine/maths/collision.h"
#include "engine/maths/calc.h"

namespace Engine
{
    bool Collision::intersects(const Rect& r, const Circ& c)
    {
        return 
            r.contains(c.center) ||
            c.intersects(r.bottom()) ||
            c.intersects(r.top()) ||
            c.intersects(r.left()) ||
            c.intersects(r.right());
    }

    Vec2 Collision::srect_drect(const Rect& sr, const Rect& dr)
    {
        if (!sr.intersects(dr))
        {
            return Vec2();
        }

        Vec2 right_push = Vec2(sr.x + sr.w - dr.x, 0.0f);
        Vec2 left_push = Vec2(sr.x - (dr.x + dr.w), 0.0f);
        Vec2 up_push = Vec2(0.0f, sr.y + sr.h - dr.y);
        Vec2 down_push = Vec2(0.0f, sr.y - (dr.y + dr.h));

        Vec2 min_x = right_push.x < Calc::abs(left_push.x) ? right_push : left_push;
        Vec2 min_y = up_push.y < Calc::abs(down_push.y) ? up_push : down_push;

        return Calc::abs(min_x.x) < Calc::abs(min_y.y) ? min_x : min_y; 
    }


    Vec2 Collision::srect_dcirc(const Rect& sr, const Circ& dc)
    {
        return Vec2();
    }
}
