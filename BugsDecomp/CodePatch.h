
// Class for registering and applying code patches.

#pragma once


#include "GameVersion.h"


#ifdef __cplusplus

struct CodePatchEntry;


class CodePatch
{
private:
    /// <summary>
    /// Gets the list of code patches to apply.
    /// </summary>
    /// <returns>Code patch list</returns>
    static std::vector<CodePatchEntry> &codePatches();
    /// <summary>
    /// Patches some bytes into code space.
    /// </summary>
    /// <param name="dst">Target address to patch</param>
    /// <param name="src">Array of bytes to patch in</param>
    /// <param name="size">Number of bytes to patch</param>
    /// <returns>Whether the patch was applied</returns>
    static bool patchCodeBytes(void *dst, const void *src, size_t size);
public:
    /// <summary>
    /// Applies all registered code patches for the current game version.
    /// 
    /// NOTE: Patches are not reverted on failure; this will likely lead to a
    /// game crash, and is (somewhat) intentional.
    /// </summary>
    /// <returns>Whether all patches were successfully applied</returns>
    static bool applyAll();

public:
    /// <summary>
    /// Creates and registers a new code patch entry.
    /// </summary>
    /// <param name="patchAddr">Address to patch</param>
    /// <param name="tgtAddr">Target of the patched jump instruction</param>
    /// <param name="ver">Version of the game to apply the patch for</param>
    CodePatch(DWORD patchAddr, DWORD tgtAddr, GameVersion ver);
};

#endif


#ifdef REGISTER_PATCHES

// helper macros for registering code patches
#define PATCH_CODE_1_0(patchAddr, tgtAddr) \
    static const CodePatch _patch_##patchAddr##_1_0( \
        patchAddr, \
        reinterpret_cast<DWORD>(tgtAddr), \
        GameVersion::Version1_0 \
    )
#define PATCH_CODE_1_6(patchAddr, tgtAddr) \
    static const CodePatch _patch_##patchAddr##_1_6( \
        patchAddr, \
        reinterpret_cast<DWORD>(tgtAddr), \
        GameVersion::Version1_6 \
    )
#define PATCH_CODE(patchAddr_1_0, patchAddr_1_6, tgtAddr) \
    PATCH_CODE_1_0(patchAddr_1_0, tgtAddr); \
    PATCH_CODE_1_6(patchAddr_1_6, tgtAddr)

#else

#define PATCH_CODE_1_0(patchAddr, tgtAddr)
#define PATCH_CODE_1_6(patchAddr, tgtAddr)
#define PATCH_CODE(patchAddr_1_0, patchAddr_1_6, tgtAddr)

#endif
