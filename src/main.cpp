#include <iostream>
#include "platform.h"

int main() 
{
    using namespace Engine;

    if (Platform::init())
    {
        while (Platform::update())
        {
            if (Platform::mouse_button_state(MouseButton::Left).pressed)
            {
                std::cout << "Pressed!" << '\n';
            }

            /* if (Platform::mouse_button_state(MouseButton::Left).released) */
            /* { */
            /*     std::cout << "Released!" << '\n'; */
            /* } */

            /* if (Platform::mouse_button_state(MouseButton::Left).down) */
            /* { */
            /*     std::cout << "Down!" << '\n'; */
            /* } */
        }
    }

    Platform::shutdown();
}
