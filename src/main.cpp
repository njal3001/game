#include <iostream>
#include "platform.h"
#include "input.h"

int main() 
{
    using namespace Engine;

    if (Platform::init())
    {
        while (Platform::update())
        {
            if (Input::mouse_button_state(MouseButton::Right).pressed)
            {
                std::cout << "Pressed!" << '\n';
            }

            if (Input::mouse_button_state(MouseButton::Right).released)
            {
                std::cout << "Released!" << '\n';
            }
        }
    }

    Platform::shutdown();
}
