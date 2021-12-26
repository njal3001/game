#include "sb/game.h"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include "engine/platform.h"
#include "engine/input.h"
#include "engine/graphics/renderer.h"
#include "engine/maths/calc.h"
#include "engine/maths/circ.h"
#include "engine/graphics/font.h"
#include "engine/log.h"
#include "engine/maths/collision.h"
#include "sb/boxcollider.h"
#include "sb/circlecollider.h"
#include "sb/physicsbody.h"

namespace SB
{
    using namespace Engine;

    Game::Game(const unsigned int target_fps)
        : m_prev_ticks(0), m_elapsed(0.0f)
    {
        m_target_ticks = (uint64_t)((1000.0f / (float)target_fps) * Platform::ticks_per_ms);
    }

    void Game::limit_fps()
    {
        uint64_t current_ticks = Platform::ticks();
        uint64_t tick_diff = current_ticks - m_prev_ticks;
        m_elapsed = tick_diff / (1000.0f * Platform::ticks_per_ms);

        if (m_target_ticks > tick_diff)
        {
            uint32_t ms = (m_target_ticks - tick_diff) / Platform::ticks_per_ms;
            Platform::sleep(ms);
        }

        m_prev_ticks = current_ticks;
    }

    void Game::run()
    {
        using namespace Engine;

        if (Platform::init())
        {
            Renderer renderer;

            Rect scene_size = Rect(0.0f, 0.0f, 128.0f, 128.0f);
            Mat4x4 matrix = Mat4x4::create_ortho(scene_size.x, scene_size.x + scene_size.w, scene_size.y, scene_size.y + scene_size.h, -1.0f, 1.0f);
            Color clear_color(0, 0, 0, 255);

            CircleCollider player_collider = CircleCollider(32.0f);
            PhysicsBody player(Vec2(16.0f, 16.0f), 10.0f, 0.8f, &player_collider);

            BoxCollider obst1_collider = BoxCollider(Vec2(16.0f, 16.0f));
            PhysicsBody obst1(Vec2(32.0f, 32.0f), 120.0f, 0.9f, &obst1_collider);

            CircleCollider obst2_collider = CircleCollider(16.0f);
            PhysicsBody obst2(Vec2(64.0f, 64.0f), 200.0f, 0.9f, &obst2_collider);

            std::vector<PhysicsBody*> physics_bodies;
            physics_bodies.push_back(&player);
            physics_bodies.push_back(&obst1);
            physics_bodies.push_back(&obst2);

            struct Collision
            {
                PhysicsBody* o1;
                PhysicsBody* o2;
                Vec2 displacement;
            };


            // Initialize before first update
            m_prev_ticks = Platform::ticks();

            while (Platform::update())
            {
                limit_fps();

                Vec2 vel;
                Vec2 dir;
                if (Input::key_state (Key::Left).down)                
                {
                    dir.x -= 1;      
                }                    
                if (Input::key_state(Key::Right).down)
                {                    
                    dir.x += 1;      
                }
                if (Input::key_state(Key::Down).down) 
                {
                    dir.y -= 1;
                }
                if (Input::key_state(Key::Up).down)
                {
                    dir.y += 1;
                }

                if (dir.len_squared() > 0.0f)
                {
                    vel = dir.norm() * 32;
                }

                player.vel = vel;

                for (auto pb : physics_bodies)
                {
                    pb->update(m_elapsed);
                }

                std::vector<Collision> collisions;

                // Static collisions
                for (size_t i = 0; i < physics_bodies.size(); i++)
                {
                    for (size_t j = i + 1; j < physics_bodies.size(); j++)
                    {
                        PhysicsBody* o1 = physics_bodies.at(i);
                        PhysicsBody* o2 = physics_bodies.at(j);
                        Vec2 disp = o1->collider->static_displacement(o1->pos, o2->pos, *o2->collider);

                        // Collision occured
                        if (disp != Vec2::zero)
                        {
                            // TODO: Find actual contact point, store time of collision for more accurate time step
                            o1->pos += disp;
                            collisions.push_back({o1, o2, disp});
                        }
                    }
                }

                // Elastic collisions
                for (const auto& c : collisions)
                {
                    // Source: https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_Balls1.cpp
                    // TODO: Find out how this works...
                    const Vec2 norm = c.displacement.norm();

                    const Vec2 tan = Vec2(-norm.y, norm.x);

                    float dp_tan1 = c.o1->vel.dot(tan);
                    float dp_tan2 = c.o2->vel.dot(tan);

                    float dp_norm1 = c.o1->vel.dot(norm);
                    float dp_norm2 = c.o2->vel.dot(norm);

                    float m1 = (dp_norm1 * (c.o1->mass - c.o2->mass) + 2.0f * c.o2->mass * dp_norm2) / (c.o1->mass + c.o2->mass);
                    float m2 = (dp_norm2 * (c.o2->mass - c.o1->mass) + 2.0f * c.o1->mass * dp_norm1) / (c.o1->mass + c.o2->mass);

                    c.o1->vel = tan * dp_tan1 + norm * m1;
                    c.o2->vel = tan * dp_tan2 + norm * m2;
                    /* const Vec2 vel_diff = (c.o1->vel - c.o2->vel);// TODO: Should this be normalized? */
                    /* const float p = 2.0f * norm.dot(vel_diff) / (c.o1->mass + c.o2->mass); */

                    /* c.o1->vel -= norm * p * c.o2->mass; */
                    /* c.o2->vel += norm * p * c.o1->mass; */
                }

                // Camera
                //renderer.push_matrix(Mat3x3::create_translation(player.pos + scene_size.center()));

                renderer.begin();

                for (const auto pb : physics_bodies)
                {
                    pb->collider->draw(pb->pos, &renderer);
                }

                renderer.end();

                Graphics::clear(clear_color);
                renderer.render(matrix);

                //renderer.pop_matrix();

                Platform::present();
            }
        }

        Platform::shutdown();
    }
}
