#include "graphics/subtexture.h"
#include <assert.h>

namespace Engine
{
    Subtexture::Subtexture(const std::shared_ptr<Texture>& texture_ref, const Rect& source)
        : m_texture_ref(texture_ref), m_source(source)
    {
        float tex_w = texture_ref->width();
        float tex_h = texture_ref->height();

        // Check bounds
        assert(source.x >= 0 && source.x + source.w <= tex_w && 
                source.y >= 0 && source.y + source.h <= tex_h);

        m_tex_coords[0] = Vec2(source.x / tex_w, source.y / tex_h);
        m_tex_coords[1] = Vec2(source.x / tex_w, (source.y + source.h) / tex_h);
        m_tex_coords[2] = Vec2((source.x + source.w) / tex_w, (source.y + source.h) / tex_h);
        m_tex_coords[3] = Vec2((source.x + source.w) / tex_w, source.y / tex_h);
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
}
