#include "sb/mover.h"
#include "sb/collider.h"
#include <vector>
#include "engine/maths/calc.h"

namespace SB
{
    using namespace Engine;

    Mover::Mover()
        : collider(nullptr), stop_on_collide(true), stop_mask(Mask::Solid), on_hit(nullptr)
    {}

    void Mover::update(const float elapsed)
    {
        // Move entity
        m_entity->pos += vel * elapsed;

        // Resolve collisions
        if (collider && stop_mask != Mask::None)
        {
            std::vector<Collider*> stop_colliders;
            scene()->all(&stop_colliders, stop_mask);

            // Check if colliding with any 
            for (const auto other : stop_colliders)
            {
                if (other != collider)
                {
                    const Vec2 disp = collider->displace(*other);

                    // Collision occured if displacement is not zero
                    if (disp != Vec2::zero)
                    {
                        m_entity->pos += disp;
                        const Vec2 disp_norm = disp.norm();

                        if (stop_on_collide)
                        {
                            // Change velocity after collision

                            // Check if other collider is moving
                            auto omover = other->get<Mover>();
                            if (omover)
                            {
                                const Vec2 vel_norm = vel.norm();
                                const Vec2 ovel_norm = omover->vel.norm();
                                const float dot = ovel_norm.dot(vel_norm);
                                
                                // TODO: Implement velocity correction for this case.
                                // Should the velocity of both entities be changed?
                                // Velocity is reduced in both the direction of the other
                                // colliders velocity and the collision normal
                            }
                            else
                            {
                                // Other collider is static, velocity is reduced
                                // in the direction of the collision normal
                                const Vec2 vel_norm = vel.norm();
                                const float dot = disp_norm.dot(vel_norm);
                                
                                if (dot < 0.0f)
                                {
                                    vel -= disp_norm * dot * vel.len();
                                }
                            }
                        }

                        if (on_hit)
                        {
                            on_hit(this, other, disp_norm);
                        }
                    }
                }
            }
        }
    }
}
