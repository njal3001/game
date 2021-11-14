#pragma once
#include <SDL2/SDL.h>
#include "graphics/graphics.h"

namespace Engine
{
    class Platform
    {
        friend class Graphics;

    private:
        static SDL_Window* g_window;

    public:
        static bool init();

        static bool update();

        static void present();

        static void shutdown();

    private:
        Platform();

        static void* create_gl_context();
        static void destroy_gl_context(void* context);
    };
}
