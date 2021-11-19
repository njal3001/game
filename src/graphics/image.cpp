#define STB_IMAGE_IMPLEMENTATION // TODO: Might not be the right place for this
#include "third_party/stb_image.h"
#include "graphics/image.h"

namespace Engine
{

    Image::Image(const std::string& filename)
    {
        int x, y, comp;
        unsigned char* data = stbi_load(filename.c_str(), &x, &y, &comp, 0);

        assert(data);

        m_pixels = (Color*)data;
        m_width = x;
        m_height = y;
    }

    Image::~Image()
    {
        stbi_image_free(m_pixels);
    }

    const Color* Image::pixels() const
    {
        return m_pixels;
    }

    unsigned int Image::width() const
    {
        return m_width;
    }

    unsigned int Image::height() const
    {
        return m_height;
    }
}
