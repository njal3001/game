#include "graphics/font.h"
#include <string>
#include "platform.h"
#include "log.h"
#include <assert.h>

namespace Engine
{
    Font::Font(const std::string& file_path, const unsigned int height)
        : m_height(height)
    {
        if (FT_New_Face(Graphics::get_font_lib(), file_path.c_str(), 0, &m_face))
        {
            Log::error("Could not load font: " + file_path);
            assert(false);
        }

        FT_Set_Pixel_Sizes(m_face, 0, height);

        // TODO: Hardcoded for now
        unsigned int max_atlas_size = 1024;
        unsigned char pixels[max_atlas_size * max_atlas_size];
        unsigned int px = 0;
        unsigned int py = 0;

        for (unsigned char ch = 0; ch < 128; ch++)
        {
            if (FT_Load_Char(m_face, ch, FT_LOAD_RENDER))
            {
                Log::warn("Could not load glyph: " + std::to_string(ch));
                continue;
            }

            FT_GlyphSlot glyph = m_face->glyph;
            FT_Bitmap bmp = m_face->glyph->bitmap;

            Character character;
            character.advance = glyph->advance.x;
            character.offset_x = glyph->bitmap_left;
            character.offset_y = glyph->bitmap_top;
            character.subtexture.set_source(Rect(px, py, bmp.width, bmp.rows));

            if (px + bmp.width > max_atlas_size)
            {
                py += m_height;
                px = 0;
            }

            if (py + bmp.rows > max_atlas_size)
            {
                // TODO: Create multiple textures instead
                Log::warn("Max font atlas height of " + std::to_string(max_atlas_size) + 
                        " reached!");
                break;
            }

            // Get character pixels
            unsigned char* out = &pixels[(int)(py * max_atlas_size + px)];
            memcpy(out, bmp.buffer, bmp.rows * bmp.pitch);
        }

        m_atlas = std::make_shared<Texture>(max_atlas_size, max_atlas_size, &pixels[0], TextureFormat::R);

        for (unsigned char ch = 0; ch < 128; ch++)
        {
            m_characters[ch].subtexture.set_texture_ref(m_atlas);
        }
    }

    unsigned int Font::height() const
    {
        return m_height;
    }

    const Character& Font::get_character(const unsigned char ch) const
    {
        return m_characters[ch];
    }

    const Character& Font::operator[](const unsigned char ch) const
    {
        return m_characters[ch];
    }

    Font::~Font()
    {
        FT_Done_Face(m_face);
    }
}
