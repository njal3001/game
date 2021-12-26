#include "engine/maths/collision.h"
#include "engine/maths/rect.h"
#include "engine/maths/circ.h"
#include "engine/maths/calc.h"

namespace Engine
{
    bool Collision::intersects(const Rect& r1, const Rect& r2)
    {
        return 
            r1.x <= r2.x + r2.w &&
            r1.x + r1.h >= r2.x &&
            r1.y <= r2.y + r2.h &&
            r1.y + r1.h >= r2.y;
    }

    bool Collision::intersects(const Circ& c1, const Circ& c2)
    {
        const float r_sum = c1.radius + c2.radius;
        return c1.center.distance_squared(c2.center) <= r_sum * r_sum;
    }

    bool Collision::intersects(const Rect& r, const Circ& c)
    {
        const float nx = Calc::clamp(r.x, r.x + r.w, c.center.x);
        const float ny = Calc::clamp(r.y, r.y + r.h, c.center.y);

        return c.center.distance_squared(Vec2(nx, ny)) <= c.radius * c.radius;
    }

    // TODO: Probably not the best way to do this
    Vec2 Collision::srect_drect(const Rect& sr, const Rect& dr)
    {
        if (!intersects(sr, dr))
        {
            return Vec2::zero;
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
        const float nx = Calc::clamp(sr.x, sr.x + sr.w, dc.center.x);
        const float ny = Calc::clamp(sr.y, sr.y + sr.h, dc.center.y);

        const Vec2 n = Vec2(nx, ny) - dc.center;
        const float len = n.len();

        if (len > dc.radius)
        {
            return Vec2();
        }

        return -n.norm() * (dc.radius - len);
    }

    Vec2 Collision::scirc_dcirc(const Circ & sc, const Circ& dc)
    {
        const Vec2 diff = dc.center - sc.center;

        const float diff_len = diff.len();
        if (diff_len > sc.radius + dc.radius)
        {
            return Vec2();
        }

        return diff.norm() * (sc.radius + dc.radius - diff_len);
    }
}
