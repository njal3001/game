#include "game/game.h"
#include <iostream>
#include <memory>
#include <string>
#include "engine/platform.h"
#include "engine/input.h"
#include "engine/graphics/renderer.h"
#include "engine/maths/calc.h"
#include "engine/graphics/font.h"
#include "engine/log.h"

namespace Game
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

            Image img(Platform::app_path() + "test_texture.png");
            std::shared_ptr<Texture> texture = std::make_shared<Texture>(img, TextureFormat::RGBA);
            Subtexture sub(texture, Rect(0.0f, 0.0f, 8.0f, 8.0f));
            Subtexture sub2(texture, Rect(4.0f, 4.0f, 4.0f, 4.0f));

            Mat4x4 matrix = Mat4x4::create_ortho(0.0f, 256.0f, 0.0f, 256.0f, -1.0f, 1.0f);
            Color clear_color(0, 0, 0, 255);

            Font font(Platform::app_path() + "/kongtext.ttf", 32);

            Vec2 cam = Vec2(0.0f, 0.0f);

            while (Platform::update())
            {
                limit_fps();

                if (Input::key_state (Key::Left).down)                
                {
                    cam.x -= 1;      
                }                    
                if (Input::key_state(Key::Right).down)
                {                    
                    cam.x += 1;      
                }
                if (Input::key_state(Key::Down).down) 
                {
                    cam.y -= 1;
                }
                if (Input::key_state(Key::Up).down)
                {
                    cam.y += 1;
                }

                renderer.push_matrix(Mat3x3::create_translation(-cam));

                renderer.begin();
                renderer.tex(sub, Vec2(0.0f, 0.0f), Vec2(32, 32), Color::white);
                renderer.str(font, "abcd\npqerk!", Vec2(32, 54), Color::white);
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
