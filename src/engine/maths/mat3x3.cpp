#include "engine/maths/mat3x3.h"
#include "engine/maths/calc.h"

namespace Engine
{
    Mat3x3::Mat3x3() :
        m11(0.0f), m12(0.0f), m13(0.0f),
        m21(0.0f), m22(0.0f), m23(0.0f),
        m31(0.0f), m32(0.0f), m33(0.0f)
    {}

    Mat3x3::Mat3x3(
        float m11, float m12, float m13,
        float m21, float m22, float m23,
        float m31, float m32, float m33)
        :
        m11(m11), m12(m12), m13(m13),
        m21(m21), m22(m22), m23(m23),
        m31(m31), m32(m32), m33(m33)
    {}

    const Mat3x3 Mat3x3::identity = Mat3x3(
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f);

    Mat3x3 Mat3x3::create_translation(float x, float y)
    {
        return Mat3x3(
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f , 0.0f,
                x, y, 1.0f);
    }

    Mat3x3 Mat3x3::create_translation(const Vec2& trans)
    {
        return create_translation(trans.x, trans.y);
    }

    Mat3x3 Mat3x3::create_scale(float x, float y)
    {
        return Mat3x3(
                x, 0.0f, 0.0f, 
                0.0f, y, 0.0f,
                0.0f, 0.0f, 1.0f);
    }

    Mat3x3 Mat3x3::create_scale(const Vec2& scale)
    {
        return create_scale(scale.x, scale.y);
    }
    
    Mat3x3 Mat3x3::create_rotation(float radians)
    {
        float c = Calc::cos(radians);
        float s = Calc::sin(radians);

        return Mat3x3(
                c, s, 0,
                -s, c, 0,
                0, 0, 1);
    }

    Mat3x3 Mat3x3::operator+ (const Mat3x3& rhs) const
    {
        return Mat3x3(
                m11 + rhs.m11, m12 + rhs.m12, m13 + rhs.m13,
                m21 + rhs.m21, m22 + rhs.m22, m23 + rhs.m23,
                m31 + rhs.m31, m32 + rhs.m32, m33 + rhs.m33);
    }

    Mat3x3 Mat3x3::operator- (const Mat3x3& rhs) const
    {
        return Mat3x3(
                m11 - rhs.m11, m12 - rhs.m12, m13 - rhs.m13,
                m21 - rhs.m21, m22 - rhs.m22, m23 - rhs.m23,
                m31 - rhs.m31, m32 - rhs.m32, m33 - rhs.m33);
    }

    Mat3x3 Mat3x3::operator* (const Mat3x3& rhs) const
    {
        return Mat3x3(
                m11 * rhs.m11 + m21 *  rhs.m12 + m31 * rhs.m13,
                m12 * rhs.m11 + m22 *  rhs.m12 + m32 * rhs.m13,
                m13 * rhs.m11 + m23 *  rhs.m12 + m33 * rhs.m13,
                m11 * rhs.m21 + m21 *  rhs.m22 + m31 * rhs.m23,
                m12 * rhs.m21 + m22 *  rhs.m22 + m32 * rhs.m23,
                m13 * rhs.m21 + m23 *  rhs.m22 + m33 * rhs.m23,
                m11 * rhs.m31 + m21 *  rhs.m32 + m31 * rhs.m33,
                m12 * rhs.m31 + m22 *  rhs.m32 + m32 * rhs.m33,
                m13 * rhs.m31 + m23 *  rhs.m32 + m33 * rhs.m33);
    }
}
