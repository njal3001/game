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

            const size_t map_width = 40;
            const size_t map_height = 20;

            Mat4x4 matrix = Mat4x4::create_ortho(0, map_width * tile_size,
                    0, map_height * tile_size, -1.0f, 1.0f);
            Color clear_color(0, 0, 0, 255);

            Tilemap tilemap(tile_size, map_width, map_height);

            Scene scene(&tilemap);

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
