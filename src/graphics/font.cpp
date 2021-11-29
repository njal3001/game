#include <string>
#define STB_TRUETYPE_IMPLEMENTATION
#include "third_party/stb_truetype.h"
#include "graphics/font.h"
#include "platform.h"
#include "log.h"

namespace Engine
{
    CharRange::CharRange(uint32_t from, uint32_t to)
        : from(from), to(to)
    {}

    const CharRange CharRange::ASCII = CharRange(32, 128);

    Font::Font(const std::string& file_path, const CharRange& char_range, const unsigned int size)
        : m_char_range(char_range), m_size(size)
    {
        File file = Platform::read_file(file_path);
        assert(file.data);

        m_data = file.data;

        m_font = new stbtt_fontinfo();

        auto info = (stbtt_fontinfo*)m_font;
        stbtt_InitFont(info, (unsigned char*)m_data, 0);

        // Initialize characters
        float scale = stbtt_ScaleForMappingEmToPixels(info, m_size);

        // TODO: Hardcoded for now
        unsigned int max_atlas_size = 1024;
        unsigned char pixels[max_atlas_size * max_atlas_size];
        unsigned int px = 0;
        unsigned int py = 0;

        for (uint32_t codepoint = m_char_range.from; codepoint < m_char_range.to; codepoint++)
        {
            int glyph = stbtt_FindGlyphIndex(info, codepoint);
            if (!glyph)
            {
                Log::warn("Glyph not found for codepoint: " + std::to_string(codepoint));
                continue;
            }

            if (!stbtt_IsGlyphEmpty(info, glyph))
            {
                Log::warn("Glyph (" + std::to_string(glyph) + ") is empty");
                continue;
            }

            // TODO: Why only offset for x?
            int advance, offset_x, x0, y0, x1, y1;

            stbtt_GetGlyphHMetrics(info, glyph, &advance, &offset_x);
            stbtt_GetGlyphBitmapBox(info, glyph, scale, scale, &x0, &y0, &x1, &y1);

            unsigned int w = x1 - x0;
            unsigned int h = y1 - y0;

            Vec2 offset(offset_x * scale, (float)y0);

            // TODO: Don't like having to create a dummy subtexture here
            Character ch =
            {
                .subtexture = Subtexture(nullptr, Rect()),
                .advance = advance * scale,
                .offset = offset,
            };

            m_characters[codepoint] = ch;

            if (px + w > max_atlas_size)
            {
                py += height();
                px = 0;
            }

            if (py + h > max_atlas_size)
            {
                Log::warn("Max font atlas height of " + std::to_string(max_atlas_size) + 
                        " reached!");
                break;
            }


            // Get character pixels
            unsigned char* out = &pixels[(int)(py * max_atlas_size + px)];
            stbtt_MakeGlyphBitmap(info, out, w, h, w, scale, scale, glyph);



        }
    }

    unsigned int Font::height() const
    {
        return m_ascent - m_descent;
    }

    unsigned int Font::size() const
    {
        return m_size; 
    }

    const Character& Font::get_character(const uint32_t codepoint) const
    {
        auto it = m_characters.find(codepoint);

        if (it != m_characters.end())
        {
            return it->second;
        }

        static Character empty =
        {
            .subtexture = Subtexture(nullptr, Rect()),
            .advance = 0.0f,
            .offset = Vec2(),
        };

        return empty;
    }

    const Character& Font::operator[](const uint32_t codepoint) const
    {
        return get_character(codepoint);
    }

    Font::~Font()
    {
        delete (stbtt_fontinfo*)m_font;
        delete[] m_data;
    }
}
