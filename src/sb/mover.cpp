#include "sb/mover.h"
#include "sb/collider.h"
#include <vector>

namespace SB
{
    using namespace Engine;

    Mover::Mover()
        : collider(nullptr), stop_mask(Mask::Solid), on_hit(nullptr)
    {}

    void Mover::update(const float elapsed)
    {
        // Move entity
        m_entity->pos += vel * elapsed;

        // Resolve collisions
        if (collider && stop_mask != Mask::None)
        {
            std::vector<Collider*> out;
            scene()->all(&out, stop_mask);

            for (const auto other : out)
            {
                if (other != collider)
                {
                    const Vec2 disp = collider->displace(*other);

                    if (disp != Vec2::zero)
                    {
                        m_entity->pos += disp;

                        const Vec2 prev_vel = vel;

                        // Correct velocity
                        const Vec2 disp_norm = disp.norm();
                        const Vec2 vel_norm = vel.norm();
                        const float dot = disp_norm.dot(vel_norm);
                        
                        if (dot < 0.0f)
                        {
                            vel -= disp_norm * dot * vel.len();
                        }

                        if (on_hit)
                        {
                            on_hit(this, other, disp_norm, prev_vel);
                        }
                    }
                }
            }
        }
    }
}
