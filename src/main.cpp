#include <iostream>
#include "platform.h"
#include "input.h"

int main() 
{
    using namespace Engine;

    Input inp;
    inp.key_state(Key::D0);

    if (Platform::init())
    {
        while (Platform::update())
        {
            if (Input::mouse_button_state(MouseButton::Left).pressed)
            {
                std::cout << "Pressed!" << '\n';
            }

            if (Input::mouse_button_state(MouseButton::Left).released)
            {
                std::cout << "Released!" << '\n';
            }
        }
    }

    Platform::shutdown();
}
