#pragma once
#include <memory>
#include "maths/rect.h"
#include "graphics/texture.h"

namespace Engine
{
    class Subtexture
    {
    private:
        std::shared_ptr<Texture> m_texture_ref;
        Rect m_source;
        std::array<Vec2, 4> m_tex_coords;

    public:
        Subtexture();
        Subtexture(const std::shared_ptr<Texture>& texture_ref, const Rect& source);

        void set_texture_ref(const std::shared_ptr<Texture>& texture_ref);
        void set_source(const Rect& source);
        std::shared_ptr<Texture> texture_ref() const;
        Rect source() const;
        const std::array<Vec2,4>& tex_coords() const;

    private:
        void update_tex_coords();
    };
}
