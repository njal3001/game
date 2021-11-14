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
            renderer.rect(Vec2(-0.5, -0.5), Vec2(1, 1), Color(255, 255, 255));
            renderer.render();
            Platform::present();
        }
    }

    Platform::shutdown();

}
