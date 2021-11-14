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

    private:
        Graphics();

        static bool init();
        static void shutdown();
    };
}
