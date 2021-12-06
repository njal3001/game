#include "graphics/subtexture.h"
#include <assert.h>

namespace Engine
{
    Subtexture::Subtexture()
    {}

    Subtexture::Subtexture(const std::shared_ptr<Texture>& texture_ref, const Rect& source)
        : m_texture_ref(texture_ref), m_source(source)
    {
        update_tex_coords();
    }

    void Subtexture::set_texture_ref(const std::shared_ptr<Texture>& texture_ref)
    {
        m_texture_ref = texture_ref;
        update_tex_coords();
    }

    void Subtexture::set_source(const Rect& source)
    {
        m_source = source;

        if (m_texture_ref)
        {
            update_tex_coords();
        }
    }

    std::shared_ptr<Texture> Subtexture::texture_ref() const
    {
        return m_texture_ref;
    }

    Rect Subtexture::source() const
    {
        return m_source;
    }

    const std::array<Vec2,4>& Subtexture::tex_coords() const
    {
        return m_tex_coords;
    }

    void Subtexture::update_tex_coords()
    {
        float tex_w = m_texture_ref->width();
        float tex_h = m_texture_ref->height();

        // Check bounds
        assert(m_source.x >= 0 && m_source.x + m_source.w <= tex_w && 
                m_source.y >= 0 && m_source.y + m_source.h <= tex_h);

        float sx = 1.0f / (float)tex_w;
        float sy = 1.0f / (float)tex_h;

        // Half pixel correction
        float dx = 0.5f / (float)tex_w;
        float dy = 0.5f / (float)tex_h;

        m_tex_coords[0] = Vec2(m_source.x * sx + dx, m_source.y * sy + dy);
        m_tex_coords[1] = Vec2(m_source.x * sx + dx, (m_source.y + m_source.h) * sy - dy);
        m_tex_coords[2] = Vec2((m_source.x + m_source.w) * sx - dx, 
                (m_source.y + m_source.h) * sy - dy);
        m_tex_coords[3] = Vec2((m_source.x + m_source.w) * sx - dx, m_source.y * sy + dy);
    }
}
