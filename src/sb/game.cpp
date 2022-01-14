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

            Rect scene_bounds = Rect(0.0f, 0.0f, 128.0f, 128.0f);
            Mat4x4 matrix = Mat4x4::create_ortho(scene_bounds.x, scene_bounds.x + scene_bounds.w, 
                    scene_bounds.y, scene_bounds.y + scene_bounds.h, -1.0f, 1.0f);
            Color clear_color(0, 0, 0, 255);

            Scene scene(scene_bounds);

            Player::create(&scene, Vec2(16.0f, 16.0f));

            /* Enemy::create(&scene, Vec2(32.0f, 32.0f), 4.0f); */
            /* Enemy::create(&scene, Vec2(64.0f, 64.0f), 6.0f); */
            /* Enemy::create(&scene, Vec2(88.0f, 88.0f), 8.0f); */
            Enemy::create(&scene, Vec2(88.0f, 16.0f), 12.0f);

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
}
