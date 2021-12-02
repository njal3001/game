#include <iostream>
#include <memory>
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

        Mat4x4 matrix = Mat4x4::create_ortho(0.0f, 8.0f, 0.0f, 8.0f, -1.0f, 1.0f);
        Color clear_color(0, 0, 0, 255);

        //Font font("./kongtext.ttf", CharRange(33, 126), 8);

        while (Platform::update())
        {
            renderer.begin();

            renderer.end();

            Graphics::clear(clear_color);
            renderer.render(matrix);

            Platform::present();
        }
    }

    Platform::shutdown();

}
