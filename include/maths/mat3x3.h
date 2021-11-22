#pragma once
#include "maths/vec2.h"

namespace Engine
{
    struct Mat3x3
    {
        float m11;
        float m12;
        float m13;

        float m21;
        float m22;
        float m23;

        float m31;
        float m32;
        float m33;

        Mat3x3();
        Mat3x3(
            float m11, float m12, float m13,
            float m21, float m22, float m23,
            float m31, float m32, float m33);

        static const Mat3x3 identity;

        static Mat3x3 create_translation(float x, float y);
        static Mat3x3 create_translation(const Vec2& trans);

        static Mat3x3 create_scale(float x, float y);
        static Mat3x3 create_scale(const Vec2& scale);
        
        static Mat3x3 create_rotation(float radians);

        Mat3x3 operator+ (const Mat3x3& rhs) const;
        Mat3x3 operator- (const Mat3x3& rhs) const;
        Mat3x3 operator* (const Mat3x3& rhs) const;
    };
}
