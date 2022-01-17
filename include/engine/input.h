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

    enum class ControllerButton
    {
        A = 0,
        B,
        X,
        Y,
        LeftShoulder
    };

    enum class Axis
    {
        LeftX = 0,
        LeftY = 1,
        RightX = 3,
        RightY = 4
    };

    // TODO: Change to shared struct
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

    struct ControllerButtonState
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
        static std::unordered_map<ControllerButton, ControllerButtonState> g_controller;
        static std::unordered_map<Axis, float> g_controller_axis;

    public:
        static KeyState key_state(const Key key);
        static MouseButtonState mouse_button_state(const MouseButton mb);

        static ControllerButtonState controller_button_state(const ControllerButton cb);
        static float axis_state(const Axis axis);

    private:
        Input();

        static void key_pressed(const Key key);
        static void mouse_button_pressed(const MouseButton mb);
        static void controller_button_pressed(const ControllerButton cb);

        static void key_released(const Key key);
        static void mouse_button_released(const MouseButton mb);
        static void controller_button_released(const ControllerButton cb);

        static void axis_changed(const Axis axis, const float val);

        static void refresh();
    };
}
