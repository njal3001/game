#pragma once
#include <string>
#include "engine/graphics/color.h"

namespace Engine
{
    class Image
    {
    private:
        Color* m_pixels;
        unsigned int m_width;
        unsigned int m_height;

    public:
        Image(const std::string& filename);
        ~Image();

        Image(const Image& other) = delete;
        Image& operator=(const Image& other) = delete;

        const Color* pixels() const;
        unsigned int width() const;
        unsigned int height() const;
    };
}
