#pragma once
#include <GL/glew.h>
#include "image.h"

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
        Texture(Image& image);
        ~Texture();

        Texture(const Texture& other) = delete;
        Texture& operator=(const Texture& other) = delete;

        GLuint id() const;
        GLsizei width() const;
        GLsizei height() const;
    };
}
