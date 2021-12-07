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

    public:
        static bool init();

        static bool update();

        static void present();

        static std::string app_path();
        static File read_file(const std::string& file_path);

        static void shutdown();

    private:
        Platform();

        static void* create_gl_context();
        static void destroy_gl_context(void* context);
    };
}
