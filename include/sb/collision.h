#include "engine/maths/rect.h"
#include "engine/maths/vec2.h"

namespace SB
{
    namespace Collision
    {
        // Returns the shortest vector that pushes r2 out of r1
        Engine::Vec2 rect_rect(const Engine::Rect& r1, const Engine::Rect& r2);
    }
}
