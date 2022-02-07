#include "sb/mover.h"
#include "sb/collider.h"
#include <vector>

namespace SB
{
    using namespace Engine;

    CollisionManager::CollisionManager(Scene *scene)
        : m_scene(scene)
    {}

    void CollisionManager::update()
    {
        std::vector<Mover*> movers;
        m_scene->all(&movers);

        // Resolve collisions
        for (size_t i = 0; i < resolve_iterations; i++)
        {
            for (auto m : movers)
            {
                /* // Get adjacent colliders */
                /* // TODO: This should be integers, not floats */
                /* const Vec2 offsets[9] = { */
                /*     Vec2(), Vec2(1, 0), Vec2(-1, 0), Vec2(0, 1), Vec2(0, -1), */
                /*     Vec2(1, 1), Vec2(-1, -1), Vec2(1, -1), Vec2(-1, 1) */
                /* }; */ 

                /* for (auto& off : offsets) */
                /* { */
                /*     const std::vector<Collider*>* out; */
                /*     get_adjacent(&out, m->collider->pos(), off.x, off.y); */

                /*     if (!out) continue; */

                /*     for (const auto other : *out) */
                /*     { */
                /*         if ((other->mask & m->stop_mask) && other->alive() && */ 
                /*                 other != m->collider) */
                /*         { */
                /*             const Vec2 disp = m->collider->displace(*other); */

                /*             // Collision occured if displacement is not zero */
                /*             if (disp != Vec2::zero) */
                /*             { */
                /*                 const Vec2 disp_norm = disp.norm(); */

                /*                 if (!other->trigger_only) */
                /*                 { */
                /*                     // Inelastic collision, update positions and velocities */
                /*                     auto om = other->get<Mover>(); */
                /*                     if (om) */
                /*                     { */
                /*                         // Other collider is moving */
                /*                         m->entity()->pos += disp / 2.0f; */
                /*                         om->entity()->pos -= disp / 2.0f; */

                /*                         const Vec2 vel_diff = m->vel - om->vel; */
                /*                         const float p = disp_norm.dot(vel_diff); */
                /*                         if (m->stop_on_collide) */
                /*                         { */
                /*                             m->vel -= disp_norm * p * collision_elasticity; */
                /*                         } */

                /*                         if (om->stop_on_collide) */
                /*                         { */
                /*                             om->vel += disp_norm * p * collision_elasticity; */
                /*                         } */
                /*                     } */
                /*                     else */
                /*                     { */
                /*                         // Other collider is static */
                /*                         m->entity()->pos += disp; */

                /*                         if (m->stop_on_collide) */
                /*                         { */
                /*                             const float p = disp_norm.dot(m->vel); */
                /*                             m->vel -= disp_norm * p * collision_elasticity; */
                /*                         } */
                /*                     } */
                /*                 } */

                /*                 // TODO: Could be triggered in multiple iterations? */
                /*                 if (m->on_hit) */
                /*                 { */
                /*                     m->on_hit(m, other, disp_norm); */
                /*                 } */
                /*             } */
                /*         } */

                /*     } */
                /* } */

                if (m->alive() && m->collider && m->stop_mask != Mask::None)
                {
                    // TODO: Could cache this maybe?
                    std::vector<Collider*> stop_colliders;
                    m_scene->all(&stop_colliders, m->stop_mask);

                    for (const auto other : stop_colliders)
                    {
                        if (other->alive() && other != m->collider)
                        {
                            const Vec2 disp = m->collider->displace(*other);

                            // Collision occured if displacement is not zero
                            if (disp != Vec2::zero)
                            {
                                const Vec2 disp_norm = disp.norm();

                                if (!other->trigger_only)
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

    /* void CollisionManager::get_adjacent(const std::vector<Collider*>** out, */ 
    /*         const Vec2& pos, const int ox, const int oy) const */
    /* { */
    /*     const size_t x = (pos.x / tile_size) + ox; */
    /*     const size_t y = (pos.y / tile_size) + oy; */

    /*     if (x < 0 || x >= grid_width || y < 0 || y >= grid_height) */
    /*     { */
    /*         *out = nullptr; */
    /*         return; */
    /*     } */

    /*     const size_t i = x + y * grid_width; */
    /*     *out = &m_grid[i]; */
    /* } */
}
