#include "engine/maths/rect.h"
#include "engine/maths/vec2.h"
#include "engine/maths/circ.h"

namespace Engine
{
    namespace Collision
    {
        bool intersects(const Rect& r, const Circ& c);

        // Returns the shortest vector that pushes the dynamic object out of the static object
        Vec2 srect_drect(const Rect& sr, const Rect& dr);
        Vec2 srect_dcirc(const Rect& sr, const Circ& dc);
        Vec2 scirc_dcirc(const Circ & sc, const Circ& dc);
    }
}
