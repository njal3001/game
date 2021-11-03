#pragma once
#include "input.h"

namespace Engine
{
    namespace Platform
    {
        bool init();

        bool update();

        KeyState key_state(const Key key);
        MouseButtonState mouse_button_state(const MouseButton mb);

        void shutdown();
    }
}
