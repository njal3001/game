#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "engine/graphics/graphics.h"

namespace Engine
{
    struct File
    {
        std::string path;
        unsigned int size;
        char* data;
    };

    class Platform
    {
        friend class Graphics;

    private:
        static SDL_Window* g_window;
        static SDL_Joystick* g_joystick;
        static int g_joystick_index;

    public:
        static const uint64_t ticks_per_ms;

    public:
        static bool init();
        static bool update();
        static void present();
        static void shutdown();

        static void set_fullscreen(const bool val);

        static uint64_t ticks();
        static void sleep(uint32_t ms);

        static std::string app_path();
        static File read_file(const std::string& file_path);


    private:
        Platform();

        static void* create_gl_context();
        static void destroy_gl_context(void* context);

        static bool joystick_is_connected(const int id);
    };
}
