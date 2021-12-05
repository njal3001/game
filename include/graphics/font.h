#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "maths/vec2.h"
#include "graphics/texture.h"
#include "graphics/subtexture.h"
#include "graphics.h"

namespace Engine
{
    struct Character
    {
        Subtexture subtexture;
        float advance;
        Vec2 offset;

        Character();
    };

    class Font
    {
    private:
        FT_Face m_face;
        unsigned int m_height;
        std::unordered_map<unsigned char, Character> m_characters;
        std::shared_ptr<Texture> m_atlas;

    public:
        Font(const std::string& file_path, const unsigned int height);
        ~Font();

        Font(const Font& other) = delete;
        Font& operator =(const Font& other) = delete;

        unsigned int height() const;
        const Character& get_character(const unsigned char ch) const;
        const Character& operator[](const unsigned char ch) const;

    };
}
