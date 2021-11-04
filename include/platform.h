#pragma once
#include <SDL2/SDL.h>

namespace Engine
{
    class Platform
    {
    private:
        static SDL_Window* g_window;
        static SDL_Renderer* g_renderer;

    public:
        static bool init();

        static bool update();

        static void shutdown();
    };
}
