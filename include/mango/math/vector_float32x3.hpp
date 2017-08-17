/*
    MANGO Multimedia Development Platform
    Copyright (C) 2012-2017 Twilight Finland 3D Oy Ltd. All rights reserved.
*/
#pragma once

#include "vector.hpp"
#include "vector_float32x2.hpp"

namespace mango
{

    // ------------------------------------------------------------------
    // Vector<float, 3>
    // ------------------------------------------------------------------

    template <>
    struct Vector<float, 3> : VectorBase<float, 3>
    {
        template <int X, int Y>
        struct Permute2
        {
			float v[3];

			operator Vector<float, 2> () const
			{
				return Vector<float, 2>(v[X], v[Y]);
			}
        };

        template <int X, int Y, int Z>
        struct Permute3
        {
			float v[3];

			operator Vector<float, 3> () const
			{
				return Vector<float, 3>(v[X], v[Y], v[Z]);
			}
        };

        union
        {
            struct { float x, y, z; };

			Permute2<0, 0> xx;
			Permute2<1, 0> yx;
			Permute2<2, 0> zx;
			Permute2<0, 1> xy;
			Permute2<1, 1> yy;
			Permute2<2, 1> zy;
			Permute2<0, 2> xz;
			Permute2<1, 2> yz;
			Permute2<2, 2> zz;

            Permute3<0, 0, 0> xxx;
            Permute3<1, 0, 0> yxx;
            Permute3<2, 0, 0> zxx;
            Permute3<0, 1, 0> xyx;
            Permute3<1, 1, 0> yyx;
            Permute3<2, 1, 0> zyx;
            Permute3<0, 2, 0> xzx;
            Permute3<1, 2, 0> yzx;
            Permute3<2, 2, 0> zzx;
            Permute3<0, 0, 1> xxy;
            Permute3<1, 0, 1> yxy;
            Permute3<2, 0, 1> zxy;
            Permute3<0, 1, 1> xyy;
            Permute3<1, 1, 1> yyy;
            Permute3<2, 1, 1> zyy;
            Permute3<0, 2, 1> xzy;
            Permute3<1, 2, 1> yzy;
            Permute3<2, 2, 1> zzy;
            Permute3<0, 0, 2> xxz;
            Permute3<1, 0, 2> yxz;
            Permute3<2, 0, 2> zxz;
            Permute3<0, 1, 2> xyz;
            Permute3<1, 1, 2> yyz;
            Permute3<2, 1, 2> zyz;
            Permute3<0, 2, 2> xzz;
            Permute3<1, 2, 2> yzz;
            Permute3<2, 2, 2> zzz;
        };

        Vector() = default;

        Vector(float s)
        {
			x = s;
			y = s;
			z = s;
        }

        explicit Vector(float s0, float s1, float s2)
        {
			x = s0;
			y = s1;
			z = s2;
        }

		explicit Vector(const Vector<float, 2>& v, float s)
		{
			x = v.x;
			y = v.y;
			z = s;
		}

		explicit Vector(float s, const Vector<float, 2>& v)
		{
			x = s;
			y = v.x;
			z = v.y;
		}

        Vector(const Vector& v)
        {
			x = v.x;
			y = v.y;
			z = v.z;
        }

        template <int X, int Y, int Z>
        Vector(const Permute3<X, Y, Z>& p)
        {
			const float* v = p.v;
			x = v[X];
			y = v[Y];
			z = v[Z];
        }

        ~Vector()
        {
        }

        template <int X, int Y, int Z>
        Vector& operator = (const Permute3<X, Y, Z>& p)
        {
			const float* v = p.v;
			x = v[X];
			y = v[Y];
			z = v[Z];
            return *this;
        }

        Vector& operator = (float s)
        {
			x = s;
			y = s;
			z = s;
            return *this;
        }

        Vector& operator = (const Vector& v)
        {
			x = v.x;
			y = v.y;
			z = v.z;
            return *this;
        }
    };

    // ------------------------------------------------------------------
    // operators
    // ------------------------------------------------------------------

    static inline const float32x3& operator + (const float32x3& v)
    {
        return v;
    }

    static inline float32x3 operator - (const float32x3& v)
    {
        return float32x3(-v.x, -v.y, -v.z);
    }

    static inline float32x3& operator += (float32x3& a, const float32x3& b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
        return a;
    }

    static inline float32x3& operator -= (float32x3& a, const float32x3& b)
    {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;
        return a;
    }

    static inline float32x3& operator *= (float32x3& a, const float32x3& b)
    {
        a.x *= b.x;
        a.y *= b.y;
        a.z *= b.z;
        return a;
    }

    static inline float32x3& operator *= (float32x3& a, float b)
    {
        a.x *= b;
        a.y *= b;
        a.z *= b;
        return a;
    }

    static inline float32x3& operator /= (float32x3& a, const float32x3& b)
    {
        a.x /= b.x;
        a.y /= b.y;
        a.z /= b.z;
        return a;
    }

    static inline float32x3& operator /= (float32x3& a, float b)
    {
        b = 1.0f / b;
        a.x *= b;
        a.y *= b;
        a.z *= b;
        return a;
    }

    static inline float32x3 operator + (float32x3 a, float32x3 b)
    {
        float x = a.x + b.x;
        float y = a.y + b.y;
        float z = a.z + b.z;
        return float32x3(x, y, z);
    }

    static inline float32x3 operator - (float32x3 a, float32x3 b)
    {
        float x = a.x - b.x;
        float y = a.y - b.y;
        float z = a.z - b.z;
        return float32x3(x, y, z);
    }

    static inline float32x3 operator * (float32x3 a, float32x3 b)
    {
        float x = a.x * b.x;
        float y = a.y * b.y;
        float z = a.z * b.z;
        return float32x3(x, y, z);
    }

    static inline float32x3 operator / (float32x3 a, float32x3 b)
    {
        float x = a.x / b.x;
        float y = a.y / b.y;
        float z = a.z / b.z;
        return float32x3(x, y, z);
    }

    static inline float32x3 operator / (float32x3 a, float b)
    {
        float s = 1.0f / b;
        float x = a.x * s;
        float y = a.y * s;
        float z = a.z * s;
        return float32x3(x, y, z);
    }

    // ------------------------------------------------------------------
    // functions
    // ------------------------------------------------------------------

    static inline float32x3 abs(float32x3 a)
    {
        float x = std::abs(a.x);
        float y = std::abs(a.y);
        float z = std::abs(a.z);
        return float32x3(x, y, z);
    }

    static inline float square(float32x3 a)
    {
        return a.x * a.x + a.y * a.y + a.z * a.z;
    }

    static inline float length(float32x3 a)
    {
        return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    }

    static inline float32x3 normalize(float32x3 a)
    {
        float s = 1.0f / length(a);
        return a * s;
    }

    static inline float32x3 min(float32x3 a, float32x3 b)
    {
        float x = std::min(a.x, b.x);
        float y = std::min(a.y, b.y);
        float z = std::min(a.z, b.z);
        return float32x3(x, y, z);
    }

    static inline float32x3 max(float32x3 a, float32x3 b)
    {
        float x = std::max(a.x, b.x);
        float y = std::max(a.y, b.y);
        float z = std::max(a.z, b.z);
        return float32x3(x, y, z);
    }

    static inline float dot(float32x3 a, float32x3 b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static inline float32x3 cross(float32x3 a, float32x3 b)
    {
        float x = a.y * b.z - a.z * b.y;
        float y = a.z * b.x - a.x * b.z;
        float z = a.x * b.y - a.y * b.x;
        return float32x3(x, y, z);
    }

    static inline float32x3 clamp(const float32x3& a, const float32x3& amin, const float32x3& amax)
    {
        const float x = std::max(amin.x, std::min(amax.x, a.x));
        const float y = std::max(amin.y, std::min(amax.y, a.y));
        const float z = std::max(amin.z, std::min(amax.z, a.z));
        return float32x3(x, y, z);
    }

    static inline float32x3 lerp(const float32x3& a, const float32x3& b, float factor)
    {
        const float x = a.x + (b.x - a.x) * factor;
        const float y = a.y + (b.y - a.y) * factor;
        const float z = a.z + (b.z - a.z) * factor;
        return float32x3(x, y, z);
    }

    static inline float32x3 lerp(const float32x3& a, const float32x3& b, const float32x3& factor)
    {
        const float x = a.x + (b.x - a.x) * factor.x;
        const float y = a.y + (b.y - a.y) * factor.y;
        const float z = a.z + (b.z - a.z) * factor.z;
        return float32x3(x, y, z);
    }

    static inline float32x3 hmin(const float32x3& v)
    {
        const float s = std::min(std::min(v.x, v.y), v.z);
        return float32x3(s);
    }

    static inline float32x3 hmax(const float32x3& v)
    {
        const float s = std::max(std::max(v.x, v.y), v.z);
        return float32x3(s);
    }

} // namespace mango
