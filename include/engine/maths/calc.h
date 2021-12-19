#pragma once

namespace Engine
{
    namespace Calc
    {
        const float PI = 3.1415926536f;
        const float TAU = PI * 2.0f;

        float cos(float x);
        float sin(float x);
        float to_radians(float angle);

        unsigned int next_power2(unsigned int v);

        float sqrt(float x);
        float abs(float x);

        float max(const float a, const float b);
        float min(const float a, const float b);
    }
}
