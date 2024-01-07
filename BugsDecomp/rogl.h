
// OpenGL renderer code.

#pragma once


/// <summary>
/// Finds and sets a GL pixel format.
/// </summary>
/// <returns>Whether a supported pixel format was set</returns>
EXTERN_C BOOL SetGLPixelFormat(void);
PATCH_CODE(0x40df80, 0x40df20, SetGLPixelFormat);

/// <summary>
/// Loads the GL library and checks for a supported pixel format.
/// </summary>
/// <returns>Whether GL is usable</returns>
EXTERN_C BOOL TestGL(void);
PATCH_CODE(0x40e0b0, 0x40e050, TestGL);

/// <summary>
/// Unloads the GL library.
/// </summary>
EXTERN_C void UnloadGL(void);
PATCH_CODE(0x40e210, 0x40e1b0, UnloadGL);
