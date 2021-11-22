#include "maths/calc.h"
#include <math.h>

namespace Engine
{
    float Calc::cos(float x)
    {
        return cosf(x);
    }

    float Calc::sin(float x)
    {
        return sinf(x);  
    }

    float Calc::to_radians(float angle)
    {
        return angle * (M_PI / 180.0f);
    }
}
