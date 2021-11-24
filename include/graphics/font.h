#pragma once
#include <string>
#include "maths/vec2.h"

namespace Engine
{
    struct Character
    {
        int glyph_index;

        // Size in pixels
        unsigned int width;
        unsigned int height;

        // Offset to next glyph
        float advance;

        // Offset from baseline
        Vec2 offset;
    };

    class Font
    {
    private:
        void* m_font;
        unsigned char* m_data;

    public:
        Font(const std::string& file_path);
        ~Font();

        Font(const Font& other) = delete;
        Font& operator =(const Font& other) = delete;


    };
}
