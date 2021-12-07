#include "engine/graphics/font.h"
#include <string>
#include "engine/maths/calc.h"
#include "engine/platform.h"
#include "engine/log.h"
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
        const unsigned char ch_start = 32;
        const unsigned char ch_end = 127;

        const unsigned int max_atlas_size = 1024;
        unsigned char pixels[max_atlas_size * max_atlas_size];
        unsigned int px = 0;
        unsigned int py = 0;

        // Next character that needs to set it's subtexture 
        unsigned char next_ch_sub = ch_start;

        for (unsigned char ch = ch_start; ch < ch_end; ch++)
        {
            if (FT_Load_Char(m_face, ch, FT_LOAD_RENDER))
            {
                Log::warn("Could not load glyph: " + std::to_string(ch));
                continue;
            }

            FT_GlyphSlot glyph = m_face->glyph;
            FT_Bitmap* bmp = &m_face->glyph->bitmap;

            if (px + bmp->width > max_atlas_size)
            {
                py += m_height;
                px = 0;
            }

            if (py + bmp->rows > max_atlas_size)
            {
                // Buffer has been filled, create texture
                m_atlas.push_back(std::make_shared<Texture>(max_atlas_size, max_atlas_size, (unsigned char*)pixels, TextureFormat::R));

                for (unsigned char ch_sub = next_ch_sub; ch_sub < ch; ch_sub++)
                {
                    m_characters[ch_sub].subtexture.set_texture_ref(m_atlas.back());
                }

                next_ch_sub = ch;

                px = 0;
                py = 0;
            }

            Character character;
            character.advance = glyph->advance.x >> 6;
            character.offset = Vec2((float)(glyph->metrics.horiBearingX >> 6), 
                    (float)((glyph->metrics.horiBearingY - glyph->metrics.height) >> 6));
            
            // Flipping rect to get correct uvs
            character.subtexture.set_source(Rect(px, max_atlas_size - py - bmp->rows, bmp->width, bmp->rows));

            m_characters[ch] = character;

            // Add character data to buffer
            for (unsigned int row = 0; row < bmp->rows; row++)
            {
                for (unsigned int col = 0; col < bmp->width; col++)
                {
                    pixels[(py + row) * max_atlas_size + px + col] = bmp->buffer[row * bmp->pitch + col];
                }
            }

            px += bmp->width;
        }

        // Create last texture if more left
        if (next_ch_sub <= ch_end)
        {
            // Reduce texture size to what's needed
            unsigned int reduced_atlas_y = Calc::next_power2(py + m_height);

            m_atlas.push_back(std::make_shared<Texture>(max_atlas_size, reduced_atlas_y, (unsigned char*)pixels, TextureFormat::R));

            for (unsigned char ch_sub = next_ch_sub; ch_sub < ch_end; ch_sub++)
            {
                Character& character = m_characters[ch_sub];

                Rect src = character.subtexture.source();
                character.subtexture.set_source(Rect(src.x, reduced_atlas_y - (max_atlas_size - src.y), src.w, src.h));
                character.subtexture.set_texture_ref(m_atlas.back());
            }
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
