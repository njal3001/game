#pragma once
#include "renderer.h"

namespace Engine
{
    class Platform;

    class Graphics
    {
        friend class Platform;

    private:
        static void* g_context;

    public:
        static void clear(Color color);

    private:
        Graphics();

        static bool init();
        static void shutdown();
    };
}
