#include "input.h"
#include <algorithm>

namespace Engine
{
    Input::Input()
    {
        KeyState key_start_state = { false, false, false };
        m_keyboard =
        {
            { Key::D0, key_start_state },
            { Key::D1, key_start_state },
            { Key::D2, key_start_state },
        };

        MouseButtonState mb_start_state = { false, false, false };
        m_mouse =
        {
            { MouseButton::Left, mb_start_state },
            { MouseButton::Right, mb_start_state },
        };
    }

    void Input::key_pressed(const Key key)
    {
        if (m_keyboard.find(key) != m_keyboard.end())
        {
            m_keyboard[key].pressed = true;
            m_keyboard[key].down = true;
            m_keyboard[key].released = false;
        }
    }


    void Input::mouse_button_pressed(const MouseButton mb)
    {
        if (m_mouse.find(mb) != m_mouse.end())
        {
            m_mouse[mb].pressed = true;
            m_mouse[mb].down = true;
            m_mouse[mb].released = false;
        }
    }

    void Input::key_released(const Key key)
    {
        if (m_keyboard.find(key) != m_keyboard.end())
        {
            m_keyboard[key].released = true;
            m_keyboard[key].down = false;
            m_keyboard[key].pressed = false;
        }
    }

    void Input::mouse_button_released(const MouseButton mb)
    {
        if (m_mouse.find(mb) != m_mouse.end())
        {
            m_mouse[mb].released = true;
            m_mouse[mb].down = false;
            m_mouse[mb].pressed = false;
        }
    }

    KeyState Input::key_state(const Key key) const
    {
        if (m_keyboard.find(key) != m_keyboard.end())
        {
            return m_keyboard.at(key);
        }

        // TODO: Log warning
        return { false, false, false };
    }

    MouseButtonState Input::mouse_button_state(const MouseButton mb) const 
    {
        if (m_mouse.find(mb) != m_mouse.end())
        {
            return m_mouse.at(mb);
        }

        // TODO: Log warning
        return { false, false, false };
    }
        
}
