#pragma once
#include <unordered_map>
#include <SDL2/SDL.h>

namespace Engine
{
    enum class Key
    {
        D0 = SDL_SCANCODE_0,
        D1 = SDL_SCANCODE_1,
        D2 = SDL_SCANCODE_2,
    };

    enum class MouseButton
    {
        None,
        Left,
        Middle,
        Right,
    };

    struct KeyState
    {
        bool down;
        bool released;
        bool pressed;
    };

    struct MouseButtonState
    {
        bool down;
        bool released;
        bool pressed;
    };
        
    class Input
    {
    private:
        std::unordered_map<Key, KeyState> m_keyboard;
        std::unordered_map<MouseButton, MouseButtonState> m_mouse;

    public:
        Input();

        void key_pressed(const Key key);
        void mouse_button_pressed(const MouseButton mb);

        void key_released(const Key key);
        void mouse_button_released(const MouseButton mb);

        KeyState key_state(const Key key) const;
        MouseButtonState mouse_button_state(const MouseButton mb) const;
    };
}
