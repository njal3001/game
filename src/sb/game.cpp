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
#include "sb/boxcollider.h"
#include "sb/circlecollider.h"
#include "sb/entity.h"

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
        if (Platform::init())
        {
            Renderer renderer;

            Rect scene_size = Rect(0.0f, 0.0f, 128.0f, 128.0f);
            Mat4x4 matrix = Mat4x4::create_ortho(scene_size.x, scene_size.x + scene_size.w, scene_size.y, scene_size.y + scene_size.h, -1.0f, 1.0f);
            Color clear_color(0, 0, 0, 255);

            CircleCollider player_collider = CircleCollider(8.0f);
            Entity player(Vec2(16.0f, 16.0f), &player_collider);

            BoxCollider obst1_collider = BoxCollider(Vec2(16.0f, 16.0f));
            Entity obst1(Vec2(32.0f, 32.0f), &obst1_collider);

            CircleCollider obst2_collider = CircleCollider(16.0f);
            Entity obst2(Vec2(64.0f, 64.0f), &obst2_collider);

            std::vector<Entity*> entities;
            entities.push_back(&player);
            entities.push_back(&obst1);
            entities.push_back(&obst2);

            // Initialize before first update
            m_prev_ticks = Platform::ticks();

            while (Platform::update())
            {
                limit_fps();

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

                Vec2 vel;
                if (dir.len_squared() > 0.0f)
                {
                    vel = dir.norm() * 32;
                }

                player.pos += vel * m_elapsed;

                // Player collision
                for (auto e : entities)
                {
                    if (e == &player)
                    {
                        continue;
                    }

                    Vec2 disp = player.collider->static_displacement(player.pos, e->pos, *e->collider);

                    // Collision occured
                    if (disp != Vec2::zero)
                    {
                        player.pos += disp;
                    }
                }

                // Camera
                //renderer.push_matrix(Mat3x3::create_translation(player.pos + scene_size.center()));

                renderer.begin();

                for (const auto e : entities)
                {
                    e->collider->draw(e->pos, &renderer);
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
