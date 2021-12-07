#include "engine/graphics/graphics.h"
#include "engine/platform.h"
#include "engine/log.h"
#include <GL/glew.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#define APIENTRY
#endif

namespace Engine
{
	void APIENTRY gl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param)
    {
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION && type == GL_DEBUG_TYPE_OTHER)
        {
            return;
        }

        if (type == GL_DEBUG_TYPE_ERROR)
        {
            Log::error(message);
        }
        else if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
        {
            Log::warn(message);
        } 
        else
        {
            Log::info(message);
        }
    }


    void* Graphics::g_context = nullptr;
    FT_Library Graphics::g_font_library = NULL;

    bool Graphics::init()
    {
        g_context = Platform::create_gl_context();
        if (!g_context)
        {
            Log::error("Failed to create OpenGL context");
            return false;
        }

        GLenum err = glewInit();
        if (err != GLEW_OK)
        {
            Log::error((char*)glewGetErrorString(err));
            return false;
        }

        if (FT_Init_FreeType(&g_font_library))
        {
            Log::error("Could not initialize FreeType");
            return false;
        }

        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // TODO: Check if debug callback is supported
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_message_callback, nullptr);

        // TODO: Blending might become batch spesific
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return true;
    }

    void Graphics::clear(Color color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    FT_Library& Graphics::get_font_lib()
    {
        return g_font_library;
    }

    void Graphics::shutdown()
    {
        FT_Done_FreeType(g_font_library);
        Platform::destroy_gl_context(g_context);
        g_context = nullptr;
    }
}
