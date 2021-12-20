#include "engine/maths/vec2.h"

namespace Engine
{
    struct Rect;
    struct Circ;

    namespace Collision
    {
        bool intersects(const Rect& r1, const Rect& r2);
        bool intersects(const Circ& c1, const Circ& c2);
        bool intersects(const Rect& r, const Circ& c);

        Vec2 srect_drect(const Rect& sr, const Rect& dr);
        Vec2 srect_dcirc(const Rect& sr, const Circ& dc);
        Vec2 scirc_dcirc(const Circ & sc, const Circ& dc);
    }
}
