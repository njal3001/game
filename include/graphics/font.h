#pragma once
#include <string>
#include <unordered_map>
#include "maths/vec2.h"
#include "graphics/texture.h"
#include "graphics/subtexture.h"

namespace Engine
{
    struct Character
    {
        Subtexture subtexture;
        float advance;
        Vec2 offset;
    };

    struct CharRange
    {
        uint32_t from;
        uint32_t to;

        CharRange(uint32_t from, uint32_t to);

        static const CharRange ASCII;
    };

    class Font
    {
    private:
        void* m_font;
        char* m_data;
        unsigned int m_ascent;
        unsigned int m_descent;
        unsigned int m_line_gap;
        CharRange m_char_range;
        unsigned int m_size;
        std::unordered_map<uint32_t, Character> m_characters;
        std::shared_ptr<Texture> m_atlas;

    public:
        Font(const std::string& file_path, const CharRange& char_range, const unsigned int size);
        ~Font();

        Font(const Font& other) = delete;
        Font& operator =(const Font& other) = delete;

        unsigned int height() const;
        unsigned int size() const;
        const Character& get_character(const uint32_t codepoint) const;
        const Character& operator[](const uint32_t codepoint) const;

    };
}
