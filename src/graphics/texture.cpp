#include "graphics/texture.h"
#include <assert.h>

namespace Engine
{
    Texture::Texture(const unsigned int width, const unsigned int height, const unsigned char* data)
        : m_id(0), m_width(width), m_height(height)
    {
        assert(width > 0 && height > 0);
        
        glGenTextures(1, &m_id);

        glBindTexture(GL_TEXTURE_2D, m_id);
        
        // TODO: Pass texture parameters as arguments?
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // TODO: Pass format as arguments
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture::Texture(Image& image)
        : Texture(image.width(), image.height(), (unsigned char*)image.pixels())
    {}


    Texture::~Texture()
    {
        glDeleteTextures(1, &m_id);
    }

    GLuint Texture::id() const
    {
        return m_id;
    }
    
    GLsizei Texture::width() const
    {
        return m_width;
    }
    
    GLsizei Texture::height() const
    {
        return m_height;
    }
}
