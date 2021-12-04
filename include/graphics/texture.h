#pragma once
#include <GL/glew.h>
#include "image.h"

namespace Engine
{
    enum class TextureFormat
    {
        RGBA = GL_RGBA,
        R = GL_RED,
    };

    class Texture
    {
    private:
        GLuint m_id;
        GLsizei m_width, m_height;
        GLuint m_format;

    public:
        // TODO: Add texture format support
        Texture(const unsigned int width, const unsigned int height, const unsigned char* data, const TextureFormat format);
        Texture(Image& image, const TextureFormat format);
        ~Texture();

        Texture(const Texture& other) = delete;
        Texture& operator=(const Texture& other) = delete;

        GLuint format() const;
        GLuint id() const;
        GLsizei width() const;
        GLsizei height() const;
    };
}
