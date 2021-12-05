#include "graphics/font.h"
#include <string>
#include "platform.h"
#include "log.h"
#include <assert.h>

namespace Engine
{
    Character::Character()
        : advance(0.0f)
    {}

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

        for (unsigned char ch = 33; ch < 127; ch++)
        {
            if (FT_Load_Char(m_face, ch, FT_LOAD_RENDER))
            {
                Log::warn("Could not load glyph: " + std::to_string(ch));
                continue;
            }

            FT_GlyphSlot glyph = m_face->glyph;
            FT_Bitmap bmp = m_face->glyph->bitmap;

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

            Log::info(std::to_string(ch) + ": " + std::to_string(glyph->bitmap_top));

            Character character;
            character.advance = glyph->advance.x >> 6;
            character.offset = Vec2(glyph->bitmap_left, glyph->bitmap_top);
            character.subtexture.set_source(Rect(px, py, bmp.width, bmp.rows));

            m_characters[ch] = character;

            // Add character to texture pixels
            for (unsigned int row = 0; row < bmp.rows; row++)
            {
                for (unsigned int col = 0; col < bmp.width; col++)
                {
                    pixels[(py + row) * max_atlas_size + px + col] = bmp.buffer[row * bmp.pitch + col];
                }
            }

            px += bmp.width;
        }

        m_atlas = std::make_shared<Texture>(max_atlas_size, max_atlas_size, (unsigned char*)pixels, TextureFormat::R);

        for (auto& it : m_characters)
        {
            it.second.subtexture.set_texture_ref(m_atlas);
        }
    }

    unsigned int Font::height() const
    {
        return m_height;
    }

    const Character& Font::get_character(const unsigned char ch) const
    {
        auto it = m_characters.find(ch);
        if (it != m_characters.end())
        {
            return it->second;
        }

        static Character empty;

        return empty;
    }

    const Character& Font::operator[](const unsigned char ch) const
    {
        return get_character(ch);
    }

    Font::~Font()
    {
        FT_Done_Face(m_face);
    }
}
