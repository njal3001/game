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

            CircleCollider player = CircleCollider(Circ(Vec2(16.0f, 16.0f), 4.0f));
            //BoxCollider player = BoxCollider(Rect(0.0f, 0.0f, 4.0f, 4.0f));
            BoxCollider obst1 = BoxCollider(Rect(32.0f, 32.0f, 16.0f, 16.0f));
            CircleCollider obst2 = CircleCollider(Circ(Vec2(64.0f, 64.0f), 16.0f));

            std::vector<Collider*> colliders;
            colliders.push_back(&player);
            colliders.push_back(&obst1);
            colliders.push_back(&obst2);


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

                player.move(vel);

                for (size_t i = 0; i < colliders.size(); i++)
                {
                    for (size_t j = i + 1; j < colliders.size(); j++)
                    {
                        Collider* o1 = colliders.at(i);
                        Collider* o2 = colliders.at(j);
                        Vec2 disp = o1->static_displacement(*o2) / 2.0f;

                        o1->move(disp);
                        o2->move(-disp);
                    }
                }

                renderer.push_matrix(Mat3x3::create_translation(-player.center() + scene_size.center()));

                /* Color obst1_color = player.intersects(obst1) ? Color::red : Color::green; */
                /* Color obst2_color = player.intersects(obst2) ? Color::red : Color::green; */

                renderer.begin();
                //renderer.rect(player.rect, Color::blue);
                renderer.circ(player.circ, 128, Color::blue);
                renderer.rect(obst1.rect, Color::green);
                renderer.circ(obst2.circ, 128, Color::green);
                renderer.circ(Vec2(0, 0), 1.0f, 128, Color::blue);
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
