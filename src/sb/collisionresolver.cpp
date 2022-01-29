#include "sb/collisionresolver.h"
#include "sb/mover.h"
#include "sb/collider.h"

namespace SB
{
    using namespace Engine;

    CollisionResolver::CollisionResolver(Scene* scene)
        : m_scene(scene)
    {}

    void CollisionResolver::update()
    {
        std::vector<Mover*> movers;
        m_scene->all(&movers);

        // Resolve collisions
        for (size_t i = 0; i < resolve_iterations; i++)
        {
            for (auto m : movers)
            {
                if (m->collider && m->stop_mask != Mask::None)
                {
                    // TODO: Could cache this maybe?
                    std::vector<Collider*> stop_colliders;
                    m_scene->all(&stop_colliders, m->stop_mask);

                    for (const auto other : stop_colliders)
                    {
                        if (other != m->collider)
                        {
                            const Vec2 disp = m->collider->displace(*other);

                            // Collision occured if displacement is not zero
                            if (disp != Vec2::zero)
                            {
                                const Vec2 disp_norm = disp.norm();

                                if (!m->trigger_only)
                                {
                                    // Inelastic collision, update positions and velocities
                                    auto om = other->get<Mover>();
                                    if (om)
                                    {
                                        // Other collider is moving
                                        m->entity()->pos += disp / 2.0f;
                                        om->entity()->pos -= disp / 2.0f;

                                        const Vec2 vel_diff = m->vel - om->vel;
                                        const float p = disp_norm.dot(vel_diff);
                                        if (m->stop_on_collide)
                                        {
                                            m->vel -= disp_norm * p * collision_elasticity;
                                        }

                                        if (om->stop_on_collide)
                                        {
                                            om->vel += disp_norm * p * collision_elasticity;
                                        }
                                    }
                                    else
                                    {
                                        // Other collider is static
                                        m->entity()->pos += disp;

                                        if (m->stop_on_collide)
                                        {
                                            const float p = disp_norm.dot(m->vel);
                                            m->vel -= disp_norm * p * collision_elasticity;
                                        }
                                    }
                                }

                                // TODO: Could be triggered in multiple iterations?
                                if (m->on_hit)
                                {
                                    m->on_hit(m, other, disp_norm);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
