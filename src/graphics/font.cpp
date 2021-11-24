#define STB_TRUETYPE_IMPLEMENTATION
#include "third_party/stb_truetype.h"
#include "graphics/font.h"
#include "platform.h"

namespace Engine
{
    Font::Font(const std::string& file_path)
    {
        File file = Platform::read_file(file_path);
        assert(file.data);

        m_data = (unsigned char*)file.data;

        m_font = new stbtt_fontinfo();
        stbtt_InitFont((stbtt_fontinfo*)m_font, m_data, 0);



    }

    Font::~Font()
    {
        delete (stbtt_fontinfo*)m_font;
        delete[] m_data;
    }
}
