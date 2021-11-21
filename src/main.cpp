#include <iostream>
#include "platform.h"
#include "input.h"
#include "graphics/renderer.h"

int main() 
{
    using namespace Engine;


    if (Platform::init())
    {
        Renderer renderer;

        Image img(Platform::app_path() + "/test_texture.png");
        Texture tex(img);

        Mat4x4 matrix = Mat4x4::create_ortho(0.0f, 8.0f, 0.0f, 8.0f, -1.0f, 1.0f);
        Mat4x4 trans = Mat4x4::create_scale(1.001f, 1.001f, 1.0f);

        Color clear_color(0, 0, 0, 255);

        while (Platform::update())
        {
            matrix = matrix * trans;

            renderer.begin();
            renderer.tex(tex, Vec2(2.0f, 2.0f), Vec2(4.0f, 4.0f), Color(255, 255, 255));
            /* renderer.rect(Vec2(-1, -1), Vec2(1, 1), Color(255, 255, 255)); */
            renderer.end();

            Graphics::clear(clear_color);
            renderer.render(matrix);

            Platform::present();
        }
    }

    Platform::shutdown();

}
