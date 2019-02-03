/*
    MANGO Multimedia Development Platform
    Copyright (C) 2012-2016 Twilight Finland 3D Oy Ltd. All rights reserved.
*/
#pragma once

#include "../core/object.hpp"
#include "../simd/simd.hpp"
#include "surface.hpp"

namespace mango
{

    struct BlitRect
    {
        u8* destImage;
        u8* srcImage;
        int destStride;
        int srcStride;
        int width;
        int height;
    };

    class Blitter : protected NonCopyable
    {
    public:
        Format srcFormat;
        Format destFormat;

        struct Component
        {
            u32 srcMask;
            u32 destMask;
            float scale;
            float bias;
            float constant; // TODO: configure
            int offset; // TODO: configure

            u32 computePack(u32 s) const
            {
                return u32((s & srcMask) * scale + bias) & destMask;
            }
        } component[4];

        int components;
        int sampleSize;
        u32 initMask;
        u32 copyMask;

#ifdef MANGO_ENABLE_SSE2
        __m128 sseScale;
        __m128i sseSrcMask;
        __m128i sseDestMask;
        __m128i sseShiftMask;
#endif

        typedef void (*FastFunc)(u8 *, const u8 *, int);
        typedef void (*ConvertFunc)(const Blitter& blitter, const BlitRect& rect);

        FastFunc custom;
        ConvertFunc convertFunc;

        Blitter(const Format& dest, const Format& source);
        ~Blitter();

        void convert(const BlitRect& rect) const
        {
            if (convertFunc)
                convertFunc(*this, rect);
        }
    };

} // namespace mango
