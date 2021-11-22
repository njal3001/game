#pragma once 

namespace Engine
{
    struct Mat4x4 
    {
        float m11;
        float m12;
        float m13;
        float m14;

        float m21;
        float m22;
        float m23;
        float m24;

        float m31;
        float m32;
        float m33;
        float m34;

        float m41;
        float m42;
        float m43;
        float m44;

        Mat4x4();
        Mat4x4(
            float m11, float m12, float m13, float m14,
            float m21, float m22, float m23, float m24,
            float m31, float m32, float m33, float m34,
            float m41, float m42, float m43, float m44);

        static const Mat4x4 identity;

        static Mat4x4 create_ortho(float left, float right, float bottom, float top, float near, float far);
        static Mat4x4 create_translation(float x, float y, float z);
        static Mat4x4 create_scale(float x, float y, float z);

        Mat4x4 operator* (const Mat4x4& rhs) const;
    };
}
