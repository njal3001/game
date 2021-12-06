#include <iostream>
#include <memory>
#include <string>
#include "platform.h"
#include "input.h"
#include "graphics/renderer.h"
#include "maths/calc.h"
#include "graphics/font.h"

int main() 
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

        while (Platform::update())
        {
            renderer.begin();
            renderer.tex(sub, Vec2(0.0f, 0.0f), Vec2(32, 32), Color::white);
            renderer.str(font, "abcd\npqerk!", Vec2(32, 54), Color::white);
            renderer.end();

            Graphics::clear(clear_color);
            renderer.render(matrix);

            Platform::present();
        }
    }

    Platform::shutdown();

}
