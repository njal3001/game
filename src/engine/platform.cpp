#include "engine/platform.h"
#include <iostream>
#include <assert.h>
#include "engine/input.h"
#include "engine/log.h"
#include "engine/graphics/graphics.h"

namespace Engine
{
    SDL_Window* Platform::g_window = nullptr;
    SDL_Joystick* Platform::g_joystick = nullptr;
    int Platform::g_joystick_index = -1;

    const uint64_t Platform::ticks_per_ms = 1000;

    bool Platform::init()
    {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);

        g_window = SDL_CreateWindow(
            "Hello World!",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            960,
            540,
            SDL_WINDOW_OPENGL
        );

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

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
            else if (event.type == SDL_KEYDOWN)
            {
                Input::key_pressed((Key)event.key.keysym.scancode);
            }
            else if (event.type == SDL_KEYUP)
            {
                Input::key_released((Key)event.key.keysym.scancode);
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
            else if (event.type == SDL_JOYDEVICEADDED)
            {
                if (!g_joystick)
                {
                    int index = event.jdevice.which;
                    if (SDL_IsGameController(index))
                    {
                        g_joystick = SDL_JoystickOpen(index);
                        g_joystick_index = index;

                        Log::info("Controller connected");
                    }
                }
                else
                {
                    Log::warn("No support for multiple controllers!");
                }
            }
            else if (event.type == SDL_JOYDEVICEREMOVED)
            {
                if (joystick_is_connected(event.jdevice.which))
                {
                    SDL_JoystickClose(g_joystick);
                    g_joystick = nullptr;
                    g_joystick_index = -1;

                    Log::info("Controller disconnected");
                }
            }
            else if (event.type == SDL_JOYBUTTONDOWN)
            {
                if (joystick_is_connected(event.jdevice.which))
                {
                    Input::controller_button_pressed((ControllerButton)event.jbutton.button);
                }
            }
            else if (event.type == SDL_JOYBUTTONUP)
            {
                if (joystick_is_connected(event.jdevice.which))
                {
                    Input::controller_button_released((ControllerButton)event.jbutton.button);
                }
            }
            else if (event.type == SDL_JOYAXISMOTION)
            {
                if (joystick_is_connected(event.jdevice.which))
                {
                    float val;
                    if (event.jaxis.value >= 0)
                    {
                        val = event.jaxis.value / 32767.0f;
                    }
                    else
                    {
                        val = event.jaxis.value / 32768.0f;
                    }

                    Input::axis_changed((Axis)event.jaxis.axis, val);
                }
            }
        }

        return cont;
    }

    bool Platform::joystick_is_connected(const int id)
    {
        return g_joystick && SDL_JoystickInstanceID(g_joystick) == id;
    }

    void Platform::present()
    {
		SDL_GL_SwapWindow(g_window);
    }
    
    void Platform::set_fullscreen(const bool val)
    {
        if (val)
        {
            SDL_SetWindowFullscreen(g_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        }
        else
        {
            SDL_SetWindowFullscreen(g_window, 0);
        }
    }

    uint64_t Platform::ticks()
    {
        auto counter = SDL_GetPerformanceCounter();
        auto freq = (double)SDL_GetPerformanceFrequency();
        return (uint64_t)(counter * ((ticks_per_ms * 1000) / freq));
    }

    void Platform::sleep(uint32_t ms)
    {
        if (ms > 0)
        {
            SDL_Delay(ms);
        }
    }

    std::string Platform::app_path()
    {
        const char* path = SDL_GetBasePath();
        return std::string(path);
    }

    File Platform::read_file(const std::string& file_path)
    {
        File file = 
            {
                .path = file_path,
                .size = 0,
                .data = nullptr
            };

        SDL_RWops* rw = SDL_RWFromFile(file_path.c_str(), "rb");
        if (!rw)
        {
            return file;
        }

        Sint64 size = SDL_RWsize(rw);
        char* buffer = new char[size];

        Sint64 read = SDL_RWread(rw, buffer, sizeof(char), size);

        SDL_RWclose(rw);
        if (read != size)
        {
            delete[] buffer;
            return file;
        }

        file.size = (unsigned int)size;
        file.data = buffer;

        return file;
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
