#pragma once
#include <stdint.h>

namespace Engine
{
    struct Color
    {
        uint8_t r, g, b, a;

        Color();

        Color(int rgb);
        Color(int rgb, float alpha);

        Color(uint8_t r, uint8_t g, uint8_t b); 
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a); 
    };
}
