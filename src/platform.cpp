#include "platform.h"
#include <iostream>
#include "input.h"
#include "log.h"
#include "graphics/graphics.h"

namespace Engine
{
    SDL_Window* Platform::g_window = nullptr;

    bool Platform::init()
    {
        SDL_Init(SDL_INIT_VIDEO);

        g_window = SDL_CreateWindow(
            "Hello World!",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            640,
            480,
            SDL_WINDOW_OPENGL
        );

        if (!Graphics::init())
        {
            Log::error("Could not initialize Graphics module!");
            return false;
        }

        return true;
    }

    bool Platform::update()
    {
        Input::refresh();
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

                    case SDL_BUTTON_RIGHT:
                        mb = MouseButton::Right;
                        break;
                }

                Input::mouse_button_pressed(mb);
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                MouseButton mb = (MouseButton)-1;
                switch (event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        mb = MouseButton::Left;
                        break;

                    case SDL_BUTTON_RIGHT:
                        mb = MouseButton::Right;
                        break;
                }

                Input::mouse_button_released(mb);
            }
        }

        return cont;
    }

    void Platform::present()
    {
		SDL_GL_SwapWindow(g_window);
    }

    void Platform::shutdown()
    {
        Graphics::shutdown();
        SDL_DestroyWindow(g_window);
        SDL_Quit();
    }


    void* Platform::create_gl_context()
    {
        void* context = SDL_GL_CreateContext(g_window);
        if (!context)
        {
            Log::error(SDL_GetError());
        }

        return context;
    }

    void Platform::destroy_gl_context(void* context)
    {
        SDL_GL_DeleteContext(context);
    }
}
