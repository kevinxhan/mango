/*
    MANGO Multimedia Development Platform
    Copyright (C) 2012-2019 Twilight Finland 3D Oy Ltd. All rights reserved.
*/
#pragma once

#include <set>
#include <string>
#include "../core/configure.hpp"
#include "../image/compression.hpp"
#include "../window/window.hpp"

// -----------------------------------------------------------------------
// OpenGL API
// -----------------------------------------------------------------------

//#define MANGO_CORE_PROFILE

#if defined(MANGO_PLATFORM_WINDOWS)

    // -----------------------------------------------------------------------
    // WGL
    // -----------------------------------------------------------------------

    #define MANGO_CONTEXT_WGL

    #define GLEXT_PROC(proc, name) extern proc name

#ifdef MANGO_CORE_PROFILE
        #include "khronos/GL/glcorearb.h"
        #include "func/glcorearb.hpp"
    #else
        #include <GL/gl.h>
        #include "khronos/GL/glext.h"
        #include "func/glext.hpp"
    #endif

    #include "khronos/GL/wgl.h"
    #include "khronos/GL/wglext.h"
    #include "func/wglext.hpp"

    #undef GLEXT_PROC

#elif defined(MANGO_PLATFORM_OSX)

    // -----------------------------------------------------------------------
    // Cocoa
    // -----------------------------------------------------------------------

    #define MANGO_CONTEXT_COCOA

    #define GL_SILENCE_DEPRECATION /* macOS 10.14 deprecated OpenGL API */
    #include "OpenGL/gl3.h"
    #include "OpenGL/gl3ext.h"

    #define GL_GLEXT_PROTOTYPES
    #include "khronos/GL/glext.h"

#elif defined(MANGO_PLATFORM_IOS)

    // -----------------------------------------------------------------------
    // EGL
    // -----------------------------------------------------------------------

    #define MANGO_CONTEXT_EGL

    //#include <OpenGLES/ES1/gl.h>
    //#include <OpenGLES/ES1/glext.h>

	// TODO: EGL context

#elif defined(MANGO_PLATFORM_ANDROID)

    // -----------------------------------------------------------------------
    // EGL
    // -----------------------------------------------------------------------

    #define MANGO_CONTEXT_EGL

    //#include <GLES/gl.h>
    //#include <GLES/glext.h>
    //#include <GLES2/gl2.h>
    #include <GLES3/gl3.h>

	// TODO: EGL context

#elif defined(MANGO_PLATFORM_UNIX)

    // -----------------------------------------------------------------------
    // GLX
    // -----------------------------------------------------------------------

    #define MANGO_CONTEXT_GLX

    #ifdef MANGO_CORE_PROFILE
        #define GL_GLEXT_PROTOTYPES
        #include "khronos/GL/glcorearb.h"
    #else
        #define GL_GLEXT_PROTOTYPES
        #include <GL/gl.h>
        #include <GL/glext.h>
    #endif

    #include <GL/glx.h>

#else

	//#error "Unsupported OpenGL implementation."

#endif

namespace mango {
namespace opengl {

    // -----------------------------------------------------------------------
    // ContextAttribute
    // -----------------------------------------------------------------------

    struct ContextAttribute
    {
        u32 red      = 8;
        u32 green    = 8;
        u32 blue     = 8;
        u32 alpha    = 8;
        u32 depth    = 24;
        u32 stencil  = 8;
        u32 samples  = 1;
    };

	// -------------------------------------------------------------------
	// Context
	// -------------------------------------------------------------------

    class Context : public Window
    {
    protected:
        struct ContextHandle* m_context;
        std::set<std::string> m_extensions;

        void initExtensionMask();

    public:
        Context(int width, int height, const ContextAttribute* attrib = nullptr, Context* shared = nullptr);
        ~Context();

        bool isExtension(const std::string& name) const;
        bool isGLES() const;
        int getVersion() const;

        void makeCurrent();
        void swapBuffers();
        void swapInterval(int interval);
        void toggleFullscreen();
        bool isFullscreen() const;
    };

	// -------------------------------------------------------------------
	// glext
	// -------------------------------------------------------------------

    struct glExtensionMask
    {
#define GL_EXTENSION(Name) u32 Name : 1;
#include "func/glext.hpp"
#undef GL_EXTENSION
    };

    extern glExtensionMask glext;

    // -------------------------------------------------------------------
    // core
    // -------------------------------------------------------------------

    struct coreExtensionMask
    {
#define CORE_EXTENSION(Version, Name) u32 Name : 1;
#include "func/glcorearb.hpp"
#undef CORE_EXTENSION

        // custom extension flags
        u32 texture_compression_dxt1 : 1;
        u32 texture_compression_dxt3 : 1;
        u32 texture_compression_dxt5 : 1;
        u32 texture_compression_etc2 : 1;
        u32 texture_compression_eac : 1;
        u32 texture_compression_latc : 1;
        u32 texture_compression_atc : 1;
    };

    extern coreExtensionMask core;

	// -------------------------------------------------------------------
	// wglext
	// -------------------------------------------------------------------

#ifdef MANGO_CONTEXT_WGL

    struct wglExtensionMask
    {
#define WGL_EXTENSION(Name) u32 Name : 1;
#include "func/wglext.hpp"
#undef WGL_EXTENSION
    };

    extern wglExtensionMask wglext;

#endif

	// -------------------------------------------------------------------
	// glxext
	// -------------------------------------------------------------------

#ifdef MANGO_CONTEXT_GLX

    struct glxExtensionMask
    {
#define GLX_EXTENSION(Name) u32 Name : 1;
#include "func/glxext.hpp"
#undef GLX_EXTENSION
    };

    extern glxExtensionMask glxext;

#endif

	// -------------------------------------------------------------------
    // helper functions ; require active context
	// -------------------------------------------------------------------

    struct InternalFormat
    {
        GLenum internalFormat;
        Format format;
        bool srgb;
        const char* name;
    };

    bool isCompressedTextureSupported(TextureCompression compression);
    const InternalFormat* getInternalFormat(GLenum internalFormat);

} // namespace opengl
} // namespace mango
