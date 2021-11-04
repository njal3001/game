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
        friend class Platform;

    private:
        static std::unordered_map<Key, KeyState> g_keyboard;
        static std::unordered_map<MouseButton, MouseButtonState> g_mouse;

    public:
        static void init();

        static KeyState key_state(const Key key);
        static MouseButtonState mouse_button_state(const MouseButton mb);

    private:
        static void key_pressed(const Key key);
        static void mouse_button_pressed(const MouseButton mb);

        static void key_released(const Key key);
        static void mouse_button_released(const MouseButton mb);

        static void refresh();
    };
}
