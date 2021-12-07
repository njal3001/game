#pragma once
#include <unordered_map>
#include <SDL2/SDL.h>

namespace Engine
{
    enum class Key
    {
        Left = SDL_SCANCODE_LEFT,
        Right = SDL_SCANCODE_RIGHT,
        Up = SDL_SCANCODE_UP,
        Down = SDL_SCANCODE_DOWN,
        X = SDL_SCANCODE_X,
        C = SDL_SCANCODE_C,
    };

    enum class MouseButton
    {
        Left,
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
        static int key_codes[5];
        static int mouse_button_codes[3];
        static std::unordered_map<Key, KeyState> g_keyboard;
        static std::unordered_map<MouseButton, MouseButtonState> g_mouse;

    public:
        // TODO: Add axes support
        static KeyState key_state(const Key key);
        static MouseButtonState mouse_button_state(const MouseButton mb);

    private:
        Input();

        static void key_pressed(const Key key);
        static void mouse_button_pressed(const MouseButton mb);

        static void key_released(const Key key);
        static void mouse_button_released(const MouseButton mb);

        static void refresh();
    };
}
