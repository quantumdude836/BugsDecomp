
#include "stdafx.h"
#include "common.h"
#include "rogl.h"


// GL function list entry
typedef struct GL_FUNC
{
    PROC *funcPtr; // where to write the found function pointer
    const char *name; // name of the function to find
} GL_FUNC;


// GL library name; defaults to "opengl32.dll"
#define glLibName (*(char (*)[32])0x4673f0)

// list of GL functions to grab from GL library
#define glFunctions (*(GL_FUNC (*)[360])0x468750)

// whether a GL pixel format is currently set; does not seem to be read
#define isPixelFormatSet (*(BOOL *)0x4b1ab4)

#define hDC (*(HDC *)0x4b1abc)

// GL function pointers
#define pwglSetPixelFormat \
    (*(BOOL (APIENTRY **)(HDC, int, const PIXELFORMATDESCRIPTOR *))0x7c637c)
#define pwglChoosePixelFormat \
    (*(int (APIENTRY **)(HDC, const PIXELFORMATDESCRIPTOR *))0x7c6584)

// handle to loaded GL library
#define glLib (*(HMODULE *)0x9ca724)


/// <summary>
/// Loads the GL library.
///
/// This function does not directly exist in the game, but is inlined in other
/// functions.
/// </summary>
/// <returns>0 for success, or non-zero for error</returns>
static DWORD LoadGL(void)
{
    // unload library if it's already loaded
    UnloadGL();

    isPixelFormatSet = FALSE;

    // try to load the library
    glLib = LoadLibraryA(glLibName);
    if (!glLib)
        return GetLastError();

    // grab required function pointers
    for (const GL_FUNC *fn = &glFunctions[0];
        fn < &glFunctions[ARRAYSIZE(glFunctions)];
        fn++)
    {
        *fn->funcPtr = GetProcAddress(glLib, fn->name);
    }
    return 0;
}


BOOL SetGLPixelFormat(void)
{
    // look for a 16-bit RGB(A) pixel format that supports OpenGL
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.iLayerType = 0;
    pfd.nSize = sizeof pfd;
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    pfd.cColorBits = 16;
    int fmt = ChoosePixelFormat(hDC, &pfd);
    if (fmt)
    {
        if (!(pfd.dwFlags & PFD_SUPPORT_OPENGL))
            return FALSE;
        if (pfd.dwFlags & PFD_GENERIC_FORMAT)
        {
            if (!(pfd.dwFlags & PFD_GENERIC_ACCELERATED))
                return FALSE;
        }
        if (!(pfd.dwFlags & PFD_DOUBLEBUFFER))
            return FALSE;
        if (pfd.iPixelType != PFD_TYPE_RGBA)
            return FALSE;
        if (pfd.cColorBits != 16)
            return FALSE;
        if (pfd.iLayerType != 0)
            return FALSE;
        if (!SetPixelFormat(hDC, fmt, &pfd))
            return FALSE;

        isPixelFormatSet = TRUE;
        return TRUE;
    }

    // try again with GL library's wglChoosePixelFormat
    fmt = pwglChoosePixelFormat(hDC, &pfd);
    if (fmt)
    {
        if (!(pfd.dwFlags & PFD_SUPPORT_OPENGL))
            return FALSE;
        if (pfd.dwFlags & PFD_GENERIC_FORMAT)
        {
            if (!(pfd.dwFlags & PFD_GENERIC_ACCELERATED))
                return FALSE;
        }
        if (!(pfd.dwFlags & PFD_DOUBLEBUFFER))
            return FALSE;
        if (pfd.iPixelType != PFD_TYPE_RGBA)
            return FALSE;
        if (pfd.cColorBits != 16)
            return FALSE;
        if (pfd.iLayerType != 0)
            return FALSE;
        if (!pwglSetPixelFormat(hDC, fmt, &pfd))
            return FALSE;

        isPixelFormatSet = TRUE;
        return TRUE;
    }

    return FALSE;
}

BOOL TestGL(void)
{
    // NOTE: this function seems to only be called when setting config defaults,
    // which is done before the game window is created, which means hDC will
    // always be NULL (which means (wgl)ChoosePixelFormat will always fail)

    DWORD err = LoadGL();
    if (err)
        return FALSE;

    // TODO: can this be merged with SetGLPixelFormat?

    // look for a 16-bit RGB(A) pixel format that supports OpenGL
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.iLayerType = 0;
    pfd.nSize = sizeof pfd;
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    pfd.cColorBits = 16;
    int fmt = ChoosePixelFormat(hDC, &pfd);
    if (fmt)
    {
        if (!(pfd.dwFlags & PFD_SUPPORT_OPENGL))
            return FALSE;
        if (pfd.dwFlags & PFD_GENERIC_FORMAT)
        {
            if (!(pfd.dwFlags & PFD_GENERIC_ACCELERATED))
                return FALSE;
        }
        if (!(pfd.dwFlags & PFD_DOUBLEBUFFER))
            return FALSE;
        if (pfd.iPixelType != PFD_TYPE_RGBA)
            return FALSE;
        if (pfd.cColorBits != 16)
            return FALSE;
        if (pfd.iLayerType != 0)
            return FALSE;

        isPixelFormatSet = TRUE;
        return TRUE;
    }

    // try again with GL library's wglChoosePixelFormat
    fmt = pwglChoosePixelFormat(hDC, &pfd);
    if (fmt)
    {
        if (!(pfd.dwFlags & PFD_SUPPORT_OPENGL))
            return FALSE;
        if (pfd.dwFlags & PFD_GENERIC_FORMAT)
        {
            if (!(pfd.dwFlags & PFD_GENERIC_ACCELERATED))
                return FALSE;
        }
        if (!(pfd.dwFlags & PFD_DOUBLEBUFFER))
            return FALSE;
        if (pfd.iPixelType != PFD_TYPE_RGBA)
            return FALSE;
        if (pfd.cColorBits != 16)
            return FALSE;
        if (pfd.iLayerType != 0)
            return FALSE;

        isPixelFormatSet = TRUE;
        return TRUE;
    }

    return FALSE;
}

void UnloadGL(void)
{
    if (glLib)
    {
        // possible bug? none of the function pointers are cleared
        FreeLibrary(glLib);
        glLib = NULL;
    }
}
