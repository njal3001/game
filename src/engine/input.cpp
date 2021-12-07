#include "engine/input.h"
#include <algorithm>
#include <iostream>
#include <string>
#include "engine/log.h"

namespace Engine
{
    // TODO: Find a cleaner way to do this
    std::unordered_map<Key, KeyState> Input::g_keyboard =
    {
        { Key::Left, { false, false, false } },
        { Key::Right, { false, false, false } },
        { Key::Up, { false, false, false } },
        { Key::Down, { false, false, false } },
        { Key::C, { false, false, false } },
        { Key::X, { false, false, false } },
    };

    std::unordered_map<MouseButton, MouseButtonState> Input::g_mouse =
    {
        { MouseButton::Left,  { false, false, false } },
        { MouseButton::Right, { false, false, false } },
    };


    void Input::refresh()
    {
        for (auto& [key, state] : g_keyboard)
        {
            state.pressed = false;
            state.released = false;
        }

        for (auto& [mb, state] : g_mouse)
        {
            state.pressed = false;
            state.released = false;
        }
    }

    void Input::key_pressed(const Key key)
    {
        if (g_keyboard.find(key) != g_keyboard.end())
        {
            g_keyboard[key].pressed = true;
            g_keyboard[key].down = true;
            g_keyboard[key].released = false;
        }
    }

    void Input::mouse_button_pressed(const MouseButton mb)
    {
        if (g_mouse.find(mb) != g_mouse.end())
        {
            g_mouse[mb].pressed = true;
            g_mouse[mb].down = true;
            g_mouse[mb].released = false;
        }
    }

    void Input::key_released(const Key key)
    {
        if (g_keyboard.find(key) != g_keyboard.end())
        {
            g_keyboard[key].released = true;
            g_keyboard[key].down = false;
            g_keyboard[key].pressed = false;
        }
    }

    void Input::mouse_button_released(const MouseButton mb)
    {
        if (g_mouse.find(mb) != g_mouse.end())
        {
            g_mouse[mb].released = true;
            g_mouse[mb].down = false;
            g_mouse[mb].pressed = false;
        }
    }

    KeyState Input::key_state(const Key key)
    {
        return g_keyboard.at(key);
    }

    MouseButtonState Input::mouse_button_state(const MouseButton mb)
    {
        return g_mouse.at(mb);
    }
        
}
