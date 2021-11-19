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

        while (Platform::update())
        {
            renderer.begin();
            renderer.tex(tex, Vec2(-1.0, -1.0), Vec2(1, 1), Color(255, 255, 255));
            //renderer.rect(Vec2(-1, -1), Vec2(1, 1), Color(255, 255, 255));
            renderer.end();
            renderer.render();

            Platform::present();
        }
    }

    Platform::shutdown();

}
