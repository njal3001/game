#include "maths/mat4x4.h"

namespace Engine
{
    Mat4x4::Mat4x4() : 
        m11(0.0f), m12(0.0f), m13(0.0f), m14(0.0f),
        m21(0.0f), m22(0.0f), m23(0.0f), m24(0.0f),
        m31(0.0f), m32(0.0f), m33(0.0f), m34(0.0f),
        m41(0.0f), m42(0.0f), m43(0.0f), m44(0.0f)
    {}

    Mat4x4::Mat4x4(
            float m11, float m12, float m13, float m14,
            float m21, float m22, float m23, float m24,
            float m31, float m32, float m33, float m34,
            float m41, float m42, float m43, float m44)
        :
            m11(m11), m12(m12), m13(m13), m14(m14),
            m21(m21), m22(m22), m23(m23), m24(m24),
            m31(m31), m32(m32), m33(m33), m34(m34),
            m41(m41), m42(m42), m43(m43), m44(m44)
        {}

    const Mat4x4 Mat4x4::identity = Mat4x4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);

    Mat4x4 Mat4x4::create_ortho(float left, float right, float bottom, float top, float near, float far)
    {
        Mat4x4 res = identity;

        res.m11 = 2.0f / (right - left);
        res.m22 = 2.0f / (top - bottom);
        res.m33 = -2.0f / (far - near);

        res.m41 = -(right + left) / (right - left);
        res.m42 = -(top + bottom) / (top - bottom);
        res.m43 = -(far + near) / (far - near);

        return res;
    }

    Mat4x4 Mat4x4::create_translation(float x, float y, float z)
    {
        Mat4x4 res = identity;

        res.m41 = x;
        res.m42 = y;
        res.m43 = z;

        return res;
    }

    Mat4x4 Mat4x4::create_scale(float x, float y, float z)
    {
        Mat4x4 res = identity;

        res.m11 = x;
        res.m22 = y;
        res.m33 = z;

        return res;
    }

    Mat4x4 Mat4x4::operator* (const Mat4x4& rhs)
    {
        Mat4x4 res;

/*         res.m11 = m11 * rhs.m11 + m21 * rhs.m12 + m31 * rhs.m13 + m41 * rhs.m14; */
/*         res.m12 = m12 * rhs.m11 + m22 * rhs.m12 + m32 * rhs.m13 + m42 * rhs.m14; */
/*         res.m13 = m13 * rhs.m11 + m23 * rhs.m12 + m33 * rhs.m13 + m43 * rhs.m14; */
/*         res.m14 = m14 * rhs.m11 + m24 * rhs.m12 + m34 * rhs.m13 + m44 * rhs.m14; */

/*         res.m21 = m11 * rhs.m21 + m21 * rhs.m22 + m31 * rhs.m23 + m41 * rhs.m24; */
/*         res.m22 = m12 * rhs.m21 + m22 * rhs.m22 + m32 * rhs.m23 + m42 * rhs.m24; */
/*         res.m23 = m13 * rhs.m21 + m23 * rhs.m22 + m33 * rhs.m23 + m43 * rhs.m24; */
/*         res.m24 = m14 * rhs.m21 + m24 * rhs.m22 + m34 * rhs.m23 + m44 * rhs.m24; */

/*         res.m31 = m11 * rhs.m31 + m21 * rhs.m32 + m31 * rhs.m33 + m41 * rhs.m34; */
/*         res.m32 = m12 * rhs.m31 + m22 * rhs.m32 + m32 * rhs.m33 + m42 * rhs.m34; */
/*         res.m33 = m13 * rhs.m31 + m23 * rhs.m32 + m33 * rhs.m33 + m43 * rhs.m34; */
/*         res.m34 = m14 * rhs.m31 + m24 * rhs.m32 + m34 * rhs.m33 + m44 * rhs.m34; */

/*         res.m41 = m11 * rhs.m41 + m21 * rhs.m42 + m31 * rhs.m43 + m41 * rhs.m44; */
/*         res.m42 = m12 * rhs.m41 + m22 * rhs.m42 + m32 * rhs.m43 + m42 * rhs.m44; */
/*         res.m43 = m13 * rhs.m41 + m23 * rhs.m42 + m33 * rhs.m43 + m43 * rhs.m44; */
/*         res.m44 = m14 * rhs.m41 + m24 * rhs.m42 + m34 * rhs.m43 + m44 * rhs.m44; */

	res.m11 = m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31 + m14 * rhs.m41;
	res.m12 = m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32 + m14 * rhs.m42;
	res.m13 = m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33 + m14 * rhs.m43;
	res.m14 = m11 * rhs.m14 + m12 * rhs.m24 + m13 * rhs.m34 + m14 * rhs.m44;

	res.m21 = m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31 + m24 * rhs.m41;
	res.m22 = m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32 + m24 * rhs.m42;
	res.m23 = m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33 + m24 * rhs.m43;
	res.m24 = m21 * rhs.m14 + m22 * rhs.m24 + m23 * rhs.m34 + m24 * rhs.m44;

	res.m31 = m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31 + m34 * rhs.m41;
	res.m32 = m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32 + m34 * rhs.m42;
	res.m33 = m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33 + m34 * rhs.m43;
	res.m34 = m31 * rhs.m14 + m32 * rhs.m24 + m33 * rhs.m34 + m34 * rhs.m44;

	res.m41 = m41 * rhs.m11 + m42 * rhs.m21 + m43 * rhs.m31 + m44 * rhs.m41;
	res.m42 = m41 * rhs.m12 + m42 * rhs.m22 + m43 * rhs.m32 + m44 * rhs.m42;
	res.m43 = m41 * rhs.m13 + m42 * rhs.m23 + m43 * rhs.m33 + m44 * rhs.m43;
	res.m44 = m41 * rhs.m14 + m42 * rhs.m24 + m43 * rhs.m34 + m44 * rhs.m44;


        return res;
    }
}
