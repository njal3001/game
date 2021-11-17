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
        while (Platform::update())
        {
            renderer.begin();
            renderer.tri(Vec2(0, 1), Vec2(-0.5, 0), Vec2(0.5, 0), Color(0, 0, 255));
            renderer.end();
            renderer.render();

            Platform::present();
        }
    }

    Platform::shutdown();

}
