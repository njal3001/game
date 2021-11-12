#include "graphics/texture.h"
#include <assert.h>

namespace Engine
{
    Texture::Texture(const unsigned int width, const unsigned int height, const unsigned char* data)
        : m_id(0), m_width(width), m_height(height)
    {
        // TODO: Add logging asserts?
        assert(width > 0 && height > 0);
        
        glGenTextures(1, &m_id);

        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLuint Texture::get_id() const
    {
        return m_id;
    }
    
    GLsizei Texture::get_width() const
    {
        return m_width;
    }
    
    GLsizei Texture::get_height() const
    {
        return m_height;
    }
}
