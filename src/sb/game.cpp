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

            Rect scene_bounds = Rect(0.0f, 0.0f, 320.0f, 180.0f);
            Mat4x4 matrix = Mat4x4::create_ortho(scene_bounds.x, scene_bounds.x + scene_bounds.w,
                    scene_bounds.y, scene_bounds.y + scene_bounds.h, -1.0f, 1.0f);
            Color clear_color(0, 0, 0, 255);

            Scene scene(scene_bounds);

            Player::create(&scene, Vec2(16.0f, 16.0f));

            // Create enemies
            Charger::create(&scene, Vec2(32.0f, 32.0f));
            Charger::create(&scene, Vec2(64.0f, 64.0f));
            Charger::create(&scene, Vec2(88.0f, 88.0f));
            Charger::create(&scene, Vec2(129.0f, 16.0f));
            Charger::create(&scene, Vec2(189.0f, 16.0f));
            Charger::create(&scene, Vec2(200.0f, 16.0f));
            Charger::create(&scene, Vec2(224.0f, 16.0f));
            Charger::create(&scene, Vec2(300.0f, 16.0f));


            Rect obst1(32.0f, 64.0f, 16.0f, 16.0f);
            Rect obst2(128.0f, 64.0f, 64.0f, 16.0f);
            create_wall(&scene, obst1, false);
            create_wall(&scene, obst2, false);

            // Create walls
            {
                const float thickness = 1000.0f;

                Rect bottom(scene_bounds.x, scene_bounds.y - thickness,
                        scene_bounds.w, thickness);
                create_wall(&scene, bottom, true);

                Rect top(scene_bounds.x, scene_bounds.y + scene_bounds.h,
                        scene_bounds.w, thickness);
                create_wall(&scene, top, true);

                Rect left(scene_bounds.x - thickness, scene_bounds.y,
                        thickness, scene_bounds.h);
                create_wall(&scene, left, true);

                Rect right(scene_bounds.x + scene_bounds.w, scene_bounds.y,
                        thickness, scene_bounds.h);
                create_wall(&scene, right, true);
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

    Entity* Game::create_wall(Scene* scene, const Rect& bounds, const bool is_bounds)
    {
        Entity* e = scene->add_entity(bounds.center());

        Collider* c = new BoxCollider(Rect(0.0f, 0.0f, bounds.w, bounds.h));
        c->visible = true;
        c->mask = Mask::Solid;
        c->is_bounds = is_bounds;
        e->add(c);

        return e;
    }
}
