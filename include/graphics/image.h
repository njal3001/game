#pragma once
#include <string>
#include "color.h"

namespace Engine
{
    class Image
    {
    private:
        const Color* m_pixels;
        unsigned int m_width;
        unsigned int m_height;

    public:
        Image(const std::string& filename);
        ~Image();

        const Color* get_pixels() const;
        unsigned int get_width() const;
        unsigned int get_height() const;
    };
}
