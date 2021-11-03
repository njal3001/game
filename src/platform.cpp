#include "platform.h"
#include <SDL2/SDL.h>
#include <iostream>
#include "input.h"

namespace Engine
{
    namespace {
        SDL_Window* g_window;
        SDL_Renderer* g_renderer;
        Input g_input;
    }

    bool Platform::init()
    {
        SDL_Init(SDL_INIT_VIDEO);

        g_window = SDL_CreateWindow(
            "Hello World!",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            640,
            480,
            0
        );

        g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_SOFTWARE);
        SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(g_renderer);
        SDL_RenderPresent(g_renderer);

        return true;
    }

    bool Platform::update()
    {
        bool cont = true;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                cont = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                MouseButton mb = (MouseButton)-1;
                switch (event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        mb = MouseButton::Left;
                        break;

                    case SDL_BUTTON_MIDDLE:
                        mb = MouseButton::Middle;
                        break;

                    case SDL_BUTTON_RIGHT:
                        mb = MouseButton::Right;
                        break;
                }

                g_input.mouse_button_pressed(mb);
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                MouseButton mb = (MouseButton)-1;
                switch (event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        mb = MouseButton::Left;
                        break;

                    case SDL_BUTTON_MIDDLE:
                        mb = MouseButton::Middle;
                        break;

                    case SDL_BUTTON_RIGHT:
                        mb = MouseButton::Right;
                        break;
                }

                g_input.mouse_button_released(mb);
            }
        }

        return cont;
    }


    KeyState Platform::key_state(const Key key)
    {
        return g_input.key_state(key);
    }

    MouseButtonState Platform::mouse_button_state(const MouseButton mb)
    {
        return g_input.mouse_button_state(mb);
    }

    void Platform::shutdown()
    {
        SDL_DestroyRenderer(g_renderer);
        SDL_DestroyWindow(g_window);
        SDL_Quit();
    }
}
