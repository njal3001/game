#include "sb/game.h"
#include <iostream>
#include <memory>
#include <string>
#include "engine/platform.h"
#include "engine/input.h"
#include "engine/graphics/renderer.h"
#include "engine/maths/calc.h"
#include "engine/maths/circ.h"
#include "engine/graphics/font.h"
#include "engine/log.h"
#include "engine/maths/collision.h"

namespace SB
{
    using namespace Engine;

    Game::Game(const unsigned int target_fps)
        : m_prev_ticks(0), m_tick_diff(0)
    {
        m_target_ticks = (uint64_t)((1000.0f / (float)target_fps) * Platform::ticks_per_ms);
    }

    void Game::limit_fps()
    {
        uint64_t current_ticks = Platform::ticks();
        m_tick_diff = current_ticks - m_prev_ticks;

        if (m_target_ticks > m_tick_diff)
        {
            uint32_t ms = (m_target_ticks - m_tick_diff) / Platform::ticks_per_ms;
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
            Mat4x4 matrix = Mat4x4::create_ortho(scene_size.x, scene_size.w, scene_size.y, scene_size.h, -1.0f, 1.0f);
            Color clear_color(0, 0, 0, 255);

            Circ player = Circ(Vec2(16.0f, 16.0f), 16.0f);
            Circ obstacle = Circ(Vec2(32.0f, 32.0f), 16.0f);

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
                    vel = dir.norm();
                }

                player.center += vel;
                Vec2 push = Collision::scirc_dcirc(obstacle, player);
                player.center += push;



                renderer.push_matrix(Mat3x3::create_translation(-player.center + scene_size.center()));

                /* Color obst_color = player.intersects(obstacle) ? Color::red : Color::green; */

                renderer.begin();
                renderer.circ(player, 128, Color::blue);
                renderer.circ(obstacle, 128, Color::green);
                renderer.end();

                Graphics::clear(clear_color);
                renderer.render(matrix);

                renderer.pop_matrix();

                Platform::present();
            }
        }

        Platform::shutdown();
    }
}
