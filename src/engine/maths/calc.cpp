#include "engine/maths/calc.h"
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

    float Calc::sqrt(float x)
    {
        return sqrtf(x);
    }

    float Calc::abs(float x)
    {
        return fabs(x);
    }

    float Calc::pow(float base, float exp)
    {
        return powf(base, exp);
    }

    float Calc::max(const float a, const float b)
    {
        return a > b ? a : b;
    }

    float Calc::min(const float a, const float b)
    {
        return a < b ? a : b;
    }

    float Calc::clamp(const float min_val, const float max_val, const float val)
    {
        return max(min_val, min(max_val, val));
    }

    float Calc::approach(const float val, const float target, const float amount)
    {
        return val < target ? min(val + amount, target) : max(val - amount, target);
    }

    float Calc::lerp(const float a, const float b, const float t)
    {
        return a + (b - a) * t;
    }
}
