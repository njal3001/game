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
#include "sb/ecs.h"
#include "sb/boxcollider.h"
#include "sb/circlecollider.h"
#include "sb/player.h"
#include "sb/enemy.h"
#include "sb/charger.h"

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

            const size_t scene_width = 40;
            const size_t scene_heigth = 20;

            Mat4x4 matrix = Mat4x4::create_ortho(0, scene_width * tile_size,
                    0, scene_heigth * tile_size, -1.0f, 1.0f);
            Color clear_color(0, 0, 0, 255);

            Scene scene(tile_size, scene_width, scene_heigth);

            Player::create(&scene, Vec2(16.0f, 16.0f));

            // Create enemies
            /* Charger::create(&scene, Vec2(32.0f, 32.0f)); */
            /* Charger::create(&scene, Vec2(64.0f, 64.0f)); */
            /* Charger::create(&scene, Vec2(88.0f, 88.0f)); */
            /* Charger::create(&scene, Vec2(129.0f, 16.0f)); */
            /* Charger::create(&scene, Vec2(189.0f, 16.0f)); */
            /* Charger::create(&scene, Vec2(200.0f, 16.0f)); */
            /* Charger::create(&scene, Vec2(224.0f, 16.0f)); */
            /* Charger::create(&scene, Vec2(300.0f, 16.0f)); */


            Rect obst1(4, 8, 2, 2);
            Rect obst2(16, 8, 8, 16);
            create_wall(&scene, 4, 8, 2, 2);
            create_wall(&scene, 16, 8, 8, 16);

            // Create bounds
            {
                const int thickness = 10;

                // Bottom
                create_wall(&scene, 0, -thickness, scene_width, thickness);
                // Top
                create_wall(&scene, 0, scene_heigth, scene_width, thickness);
                // Left
                create_wall(&scene, -thickness, 0, thickness, scene_heigth);
                // Right
                create_wall(&scene, scene_width, 0, thickness, scene_heigth);
            }

            // Initialize before first update
            m_prev_ticks = Platform::ticks();

            while (Platform::update())
            {
                limit_fps();

                scene.update(m_elapsed);

                renderer.begin();
                scene.render(&renderer);

                renderer.end();

                Graphics::clear(clear_color);
                renderer.render(matrix);

                Platform::present();
            }
        }

        Platform::shutdown();
    }

    Entity* Game::create_wall(Scene* scene, const int x, const int y,
            const int w, const int h)
    {
        const Rect bounds(x * tile_size, y * tile_size, w * tile_size, h * tile_size);
        Entity* e = scene->add_entity(bounds.center());

        Collider* c = new BoxCollider(Rect(0.0f, 0.0f, bounds.w, bounds.h));
        c->visible = true;
        c->mask = Mask::Solid;
        e->add(c);

        return e;
    }
}
