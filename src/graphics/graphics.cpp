#include "graphics/graphics.h"
#include "platform.h"
#include "log.h"
#include "iostream"

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

    bool Graphics::init()
    {
        g_context = Platform::create_gl_context();
        if (!g_context)
        {
            Log::error("Failed to create OpenGL context");
            return false;
        }

        glewInit();

        // TODO: Check if debug callback is supported
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_message_callback, nullptr);

        return true;
    }

    void Graphics::shutdown()
    {
        Platform::destroy_gl_context(g_context);
        g_context = nullptr;
    }
}
