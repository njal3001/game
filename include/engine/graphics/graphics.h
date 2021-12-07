#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include "engine/graphics/color.h"

namespace Engine
{
    class Platform;

    class Graphics
    {
        friend class Platform;

    private:
        static void* g_context;
        static FT_Library g_font_library;

    public:
        static void clear(Color color);
        static FT_Library& get_font_lib();

    private:
        Graphics();

        static bool init();
        static void shutdown();
    };
}
