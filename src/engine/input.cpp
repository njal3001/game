#include "engine/input.h"
#include <algorithm>
#include <iostream>
#include <string>
#include "engine/log.h"

namespace Engine
{
    // TODO: Could do it with bits instead
    // TODO: Find a cleaner way to do this, just use arrays...
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

    std::unordered_map<ControllerButton, ControllerButtonState> Input::g_controller =
    {
        { ControllerButton::A,  { false, false, false } },
        { ControllerButton::B, { false, false, false } },
        { ControllerButton::X, { false, false, false } },
        { ControllerButton::Y, { false, false, false } },
        { ControllerButton::LeftShoulder, { false, false, false } },
        { ControllerButton::RightShoulder, { false, false, false } },
    };

    std::unordered_map<Axis, float> Input::g_controller_axis =
    {
        { Axis::LeftX, 0.0f },
        { Axis::LeftY, 0.0f },
        { Axis::RightX, 0.0f },
        { Axis::RightY, 0.0f },
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

        for (auto& [b, state] : g_controller)
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

    void Input::controller_button_pressed(const ControllerButton cb)
    {
        if (g_controller.find(cb) != g_controller.end())
        {
            g_controller[cb].pressed = true;
            g_controller[cb].down = true;
            g_controller[cb].released = false;
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

    void Input::controller_button_released(const ControllerButton cb)
    {
        if (g_controller.find(cb) != g_controller.end())
        {
            g_controller[cb].released = true;
            g_controller[cb].pressed = false;
            g_controller[cb].down = false;
        }
    }

    void Input::axis_changed(const Axis axis, const float val)
    {
        if (g_controller_axis.find(axis) != g_controller_axis.end())
        {
            g_controller_axis[axis] = val;
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

    ControllerButtonState Input::controller_button_state(const ControllerButton cb)
    {
        return g_controller.at(cb);
    }

    float Input::axis_state(const Axis axis)
    {
        return g_controller_axis.at(axis);
    }
}
