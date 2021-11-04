#include "input.h"
#include <algorithm>
#include <iostream>

namespace Engine
{
    std::unordered_map<Key, KeyState> Input::g_keyboard;
    std::unordered_map<MouseButton, MouseButtonState> Input::g_mouse;

    void Input::init()
    {
        KeyState key_start_state = { false, false, false };
        g_keyboard =
        {
            { Key::D0, key_start_state },
            { Key::D1, key_start_state },
            { Key::D2, key_start_state },
        };

        MouseButtonState mb_start_state = { false, false, false };
        g_mouse =
        {
            { MouseButton::Left, mb_start_state },
            { MouseButton::Right, mb_start_state },
        };
    }


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
        if (g_keyboard.find(key) != g_keyboard.end())
        {
            return g_keyboard.at(key);
        }

        // TODO: Log warning
        return { false, false, false };
    }

    MouseButtonState Input::mouse_button_state(const MouseButton mb)
    {
        if (g_mouse.find(mb) != g_mouse.end())
        {
            return g_mouse.at(mb);
        }

        // TODO: Log warning
        return { false, false, false };
    }
        
}
