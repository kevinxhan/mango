/*
    MANGO Multimedia Development Platform
    Copyright (C) 2012-2019 Twilight Finland 3D Oy Ltd. All rights reserved.
*/
#pragma once

#include <cassert>
#include "configure.hpp"
#include "bits.hpp"

namespace mango
{

    // --------------------------------------------------------------
    // u32 builders
    // --------------------------------------------------------------

    constexpr u32 make32le(char c0, char c1, char c2, char c3)
    {
        return (c3 << 24) | (c2 << 16) | (c1 << 8) | c0;
    }

    constexpr u32 make32be(char c0, char c1, char c2, char c3)
    {
        return (c0 << 24) | (c1 << 16) | (c2 << 8) | c3;
    }

    // --------------------------------------------------------------
    // unaligned load/store
    // --------------------------------------------------------------

    static inline u16 uload16(const void* p)
    {
        u16 value;
        std::memcpy(&value, p, sizeof(u16));
        return value;
    }

    static inline u32 uload32(const void* p)
    {
        u32 value;
        std::memcpy(&value, p, sizeof(u32));
        return value;
    }

    static inline u64 uload64(const void* p)
    {
        u64 value;
        std::memcpy(&value, p, sizeof(u64));
        return value;
    }

    static inline void ustore16(void* p, u16 value)
    {
        std::memcpy(p, &value, sizeof(u16));
    }

    static inline void ustore32(void* p, u32 value)
    {
        std::memcpy(p, &value, sizeof(u32));
    }

    static inline void ustore64(void* p, u64 value)
    {
        std::memcpy(p, &value, sizeof(u64));
    }

    // --------------------------------------------------------------
	// endian load/store
    // --------------------------------------------------------------

#ifdef MANGO_LITTLE_ENDIAN

    // load

    static inline u16 uload16le(const void* p)
    {
        return uload16(p);
    }

    static inline u32 uload32le(const void* p)
    {
        return uload32(p);
    }

    static inline u64 uload64le(const void* p)
    {
        return uload64(p);
    }

    static inline u16 uload16be(const void* p)
    {
        return byteswap(uload16(p));
    }

    static inline u32 uload32be(const void* p)
    {
        return byteswap(uload32(p));
    }

    static inline u64 uload64be(const void* p)
    {
        return byteswap(uload64(p));
    }

    // store

    static inline void ustore16le(void* p, u16 value)
    {
        ustore16(p, value);
    }

    static inline void ustore32le(void* p, u32 value)
    {
        ustore32(p, value);
    }

    static inline void ustore64le(void* p, u64 value)
    {
        ustore64(p, value);
    }

    static inline void ustore16be(void* p, u16 value)
    {
        ustore16(p, byteswap(value));
    }

    static inline void ustore32be(void* p, u32 value)
    {
        ustore32(p, byteswap(value));
    }

    static inline void ustore64be(void* p, u64 value)
    {
        ustore64(p, byteswap(value));
    }

#else

    // load

    static inline u16 uload16le(const void* p)
    {
        return byteswap(uload16(p));
    }

    static inline u32 uload32le(const void* p)
    {
        return byteswap(uload32(p));
    }

    static inline u64 uload64le(const void* p)
    {
        return byteswap(uload64(p));
    }

    static inline u16 uload16be(const void* p)
    {
        return uload16(p);
    }

    static inline u32 uload32be(const void* p)
    {
        return uload32(p);
    }

    static inline u64 uload64be(const void* p)
    {
        return uload64(p);
    }

    // store

    static inline void ustore16le(void* p, u16 value)
    {
        ustore16(p, byteswap(value));
    }

    static inline void ustore32le(void* p, u32 value)
    {
        ustore32(p, byteswap(value));
    }

    static inline void ustore64le(void* p, u64 value)
    {
        ustore64(p, byteswap(value));
    }

    static inline void ustore16be(void* p, u16 value)
    {
        ustore16(p, value);
    }

    static inline void ustore32be(void* p, u32 value)
    {
        ustore32(p, value);
    }

    static inline void ustore64be(void* p, u64 value)
    {
        ustore64(p, value);
    }

#endif

    // --------------------------------------------------------------
    // endian storage types
    // --------------------------------------------------------------

    // Interface for endian aware storage class.

    // Implements swap-on-read and swap-on-write for built-in types
    // with compile-time endianess selection. Strict aliasing is honored
    // by using char array as the underlying storage representation.
    // Compilers will recognize the short memcpy idiom and use native
    // load and store instructions in their place - alignment always works
    // with this approach.

    // The problem this solves is a new one introduced by the memory mapped I/O
    // architecture of MANGO API; client can read/write struct that "just works"

    /* Code example:
    struct Foo
    {
        uint32be a;
        uint32be b;
    };

    void bar(const char *ptr)
    {
        // ptr is pointer to some offset in a memory mapped file
        // convert it to our type Foo. Packing will always be correct as we use char storage (see above)
        const Foo *foo = reinterpret_cast<const Foo *>(ptr);

        // The endian conversion is done when we read from the variables
        uint32 a = foo->a;
        uint32 b = foo->b;
    }

    */

    namespace detail
    {

        template <typename T>
        class TypeCopy {
        protected:
            char data[sizeof(T)];

        public:
            TypeCopy() = default;

            TypeCopy(const T &value) {
                std::memcpy(data, &value, sizeof(T));
            }

            // copy-on-write
            const TypeCopy& operator = (const T &value) {
                std::memcpy(data, &value, sizeof(T));
                return *this;
            }

            // copy-on-read
            operator T () const {
                T temp;
                std::memcpy(&temp, data, sizeof(T));
                return temp;
            }
        };

        template <typename T>
        class TypeSwap {
        protected:
            char data[sizeof(T)];

        public:
            TypeSwap() = default;

            TypeSwap(const T &value) {
                T temp = byteswap(value);
                std::memcpy(data, &temp, sizeof(T));
            }

            // swap-on-write
            const TypeSwap& operator = (const T &value) {
                T temp = byteswap(value);
                std::memcpy(data, &temp, sizeof(T));
                return *this;
            }

            // swap-on-read
            operator T () const {
                T temp;
                std::memcpy(&temp, data, sizeof(T));
                return byteswap(temp);
            }
        };

    } // namespace detail

#ifdef MANGO_LITTLE_ENDIAN

    using int16le = detail::TypeCopy<s16>;
    using int32le = detail::TypeCopy<s32>;
    using int64le = detail::TypeCopy<s64>;
    using uint16le = detail::TypeCopy<u16>;
    using uint32le = detail::TypeCopy<u32>;
    using uint64le = detail::TypeCopy<u64>;
    using float16le = detail::TypeCopy<float16>;
    using float32le = detail::TypeCopy<float32>;
    using float64le = detail::TypeCopy<float64>;

    using int16be = detail::TypeSwap<s16>;
    using int32be = detail::TypeSwap<s32>;
    using int64be = detail::TypeSwap<s64>;
    using uint16be = detail::TypeSwap<u16>;
    using uint32be = detail::TypeSwap<u32>;
    using uint64be = detail::TypeSwap<u64>;
    using float16be = detail::TypeSwap<float16>;
    using float32be = detail::TypeSwap<float32>;
    using float64be = detail::TypeSwap<float64>;

#else

    using int16le = detail::TypeSwap<s16>;
    using int32le = detail::TypeSwap<s32>;
    using int64le = detail::TypeSwap<s64>;
    using uint16le = detail::TypeSwap<u16>;
    using uint32le = detail::TypeSwap<u32>;
    using uint64le = detail::TypeSwap<u64>;
    using float16le = detail::TypeSwap<float16>;
    using float32le = detail::TypeSwap<float32>;
    using float64le = detail::TypeSwap<float64>;

    using int16be = detail::TypeCopy<s16>;
    using int32be = detail::TypeCopy<s32>;
    using int64be = detail::TypeCopy<s64>;
    using uint16be = detail::TypeCopy<u16>;
    using uint32be = detail::TypeCopy<u32>;
    using uint64be = detail::TypeCopy<u64>;
    using float16be = detail::TypeCopy<float16>;
    using float32be = detail::TypeCopy<float32>;
    using float64be = detail::TypeCopy<float64>;

#endif
    
} // namespace mango
