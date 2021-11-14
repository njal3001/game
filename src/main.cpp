#include <iostream>
#include "platform.h"
#include "input.h"
#include "graphics/renderer.h"

int main() 
{
    using namespace Engine;

    if (Platform::init())
    {
        std::cout << "Trying to render" << '\n';
        Renderer renderer;
        while (Platform::update())
        {
            renderer.rect(Vec2(0,0), Vec2(1, 1), Color(0xFFFFFF));
            renderer.render();
        }
    }

    Platform::shutdown();
}
