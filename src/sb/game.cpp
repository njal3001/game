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
        : m_prev_ticks(0), m_tick_diff(0), m_elapsed(0.0f)
    {
        m_target_ticks = (uint64_t)((1000.0f / (float)target_fps) * Platform::ticks_per_ms);
    }

    void Game::limit_fps()
    {
        uint64_t current_ticks = Platform::ticks();
        m_tick_diff = current_ticks - m_prev_ticks;

        if (m_target_ticks > m_tick_diff)
        {
            m_elapsed = (m_target_ticks - m_tick_diff) / (float)Platform::ticks_per_ms; 
            Platform::sleep((uint32_t)m_elapsed);
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

            /* CircleCollider player = CircleCollider(Circ(Vec2(16.0f, 16.0f), 4.0f)); */
            /* //BoxCollider player = BoxCollider(Rect(0.0f, 0.0f, 4.0f, 4.0f)); */
            /* BoxCollider obst1 = BoxCollider(Rect(32.0f, 32.0f, 16.0f, 16.0f)); */
            /* CircleCollider obst2 = CircleCollider(Circ(Vec2(64.0f, 64.0f), 16.0f)); */

            /* std::vector<Collider*> colliders; */
            /* colliders.push_back(&player); */
            /* colliders.push_back(&obst1); */
            /* colliders.push_back(&obst2); */

            /* std::vector<PhysicsBody> physics_bodies; */
            /* physics_bodies.emplace_back(Vec2(16.0f, 16.0f), 10.0f, 0.8f, &player); */
            /* physics_bodies.emplace_back(Vec2(32.0f, 32.0f), 10.0f, 0.8f, &obst1); */
            /* physics_bodies.emplace_back(Vec2(64.0f, 64.0f), 10.0f, 0.8f, &obst2); */

            CircleCollider player = CircleCollider(4.0f);
            Vec2 player_pos = Vec2(16.0f, 16.0f);

            BoxCollider obst1 = BoxCollider(Vec2(16.0f, 16.0f));
            Vec2 obst1_pos = Vec2(32.0f, 32.0f);

            CircleCollider obst2 = CircleCollider(16.0f);
            Vec2 obst2_pos = Vec2(64.0f, 64.0f);

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


                /* player.move(vel); */

                player_pos += vel;

                /* for (size_t i = 0; i < colliders.size(); i++) */
                /* { */
                /*     for (size_t j = i + 1; j < colliders.size(); j++) */
                /*     { */
                /*         Collider* o1 = colliders.at(i); */
                /*         Collider* o2 = colliders.at(j); */
                /*         Vec2 disp = o1->static_displacement(*o2) / 2.0f; */

                /*         o1->move(disp); */
                /*         o2->move(-disp); */
                /*     } */
                /* } */

                Vec2 disp1 = player.static_displacement(player_pos, obst1_pos, obst1);
                player_pos += disp1;
                Vec2 disp2 = player.static_displacement(player_pos, obst2_pos, obst2);
                player_pos += disp2;

                renderer.push_matrix(Mat3x3::create_translation(-player_pos + scene_size.center()));

                Color obst1_color = player.Collider::intersects(player_pos, obst1_pos, obst1) ? Color::red : Color::green;
                Color obst2_color = player.Collider::intersects(player_pos, obst2_pos, obst2) ? Color::red : Color::green;
                /* Color obst2_color = player.intersects(obst2) ? Color::red : Color::green; */


                renderer.begin();
                //renderer.rect(player.rect, Color::blue);
                /* renderer.circ(player.circ, 128, Color::blue); */
                /* renderer.rect(obst1.rect, Color::green); */
                /* renderer.circ(obst2.circ, 128, Color::green); */
                /* renderer.circ(Vec2(0, 0), 1.0f, 128, Color::blue); */
                renderer.circ(player_pos, player.radius, 128, Color::blue);
                renderer.rect(obst1_pos - (obst1.size / 2.0f), obst1.size, obst1_color);
                renderer.circ(obst2_pos, obst2.radius, 128, obst2_color);
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
