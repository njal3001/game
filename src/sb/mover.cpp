#include "sb/mover.h"
#include "sb/collider.h"
#include <vector>
#include "engine/maths/calc.h"

namespace SB
{
    using namespace Engine;

    Mover::Mover()
        : collider(nullptr), stop_on_collide(true), trigger_only(false), 
        stop_mask(Mask::Solid), on_hit(nullptr)
    {}

    void Mover::update(const float elapsed)
    {
        // Move entity
        m_entity->pos += vel * elapsed;
    }
}
