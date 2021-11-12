#pragma once
#include <GL/glew.h>

namespace Engine
{
    class Texture
    {
    private:
        GLuint m_id;
        GLsizei m_width, m_height;

    public:
        // TODO: Add texture format support
        Texture(const unsigned int width, const unsigned int height, const unsigned char* data);

        GLuint get_id() const;
        GLsizei get_width() const;
        GLsizei get_height() const;
    };
}
