#include "maths/calc.h"
#include <math.h>

namespace Engine
{
    float Calc::cos(const float x)
    {
        return cosf(x);
    }

    float Calc::sin(const float x)
    {
        return sinf(x);  
    }

    float Calc::to_radians(const float angle)
    {
        return angle * (M_PI / 180.0f);
    }

    unsigned int Calc::next_power2(unsigned int v)
    {
        if (!v)
        {
            return 1;
        }

        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++;

        return v;
    }
}
