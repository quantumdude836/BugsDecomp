
#include "stdafx.h"
// code patches will be registered here
#define REGISTER_PATCHES
#include "CodePatch.h"

// include all decompiled headers to gather and register all the patches
#include "cfg.h"
#include "crt.h"
#include "fs.h"
#include "gte.h"
#include "list.h"
#include "misc.h"
#include "pad.h"
#include "rogl.h"
#include "track.h"
#include "track_prv.h"


// represents a registered code patch
struct CodePatchEntry
{
    DWORD patchAddr;
    DWORD tgtAddr;
    GameVersion ver;

    CodePatchEntry(DWORD patchAddr, DWORD tgtAddr, GameVersion ver) :
        patchAddr(patchAddr),
        tgtAddr(tgtAddr),
        ver(ver)
    {
    }
};


std::vector<CodePatchEntry> &CodePatch::codePatches()
{
    // workaround to avoid static initialization order fiasco
    static std::vector<CodePatchEntry> *list = new std::vector<CodePatchEntry>;
    return *list;
}

bool CodePatch::patchCodeBytes(void *dst, const void *src, size_t size)
{
    DWORD flOld;

    // ensure code is writable
    // TODO: track pages that have already been modified?
    BOOL res = VirtualProtect(
        dst,
        size,
        PAGE_EXECUTE_WRITECOPY,
        &flOld
    );
    if (!res)
        return false;

    // copy bytes over
    memcpy(dst, src, size);

    return true;
}

bool CodePatch::applyAll()
{
    BYTE buf[5];
    GameVersion curVer = *detectVersion();

    for (const auto &entry : codePatches())
    {
        // only apply the patch if it applies to the current running version
        if (entry.ver != curVer)
            continue;

        // 32-bit relative jump: E9 xx xx xx xx
        buf[0] = 0xe9;
        // relative offset = (target address) - (address of next instruction)
        // FIXME? unaligned write
        *reinterpret_cast<DWORD *>(&buf[1]) =
            entry.tgtAddr - (entry.patchAddr + 5);

        bool res = patchCodeBytes(
            reinterpret_cast<void *>(entry.patchAddr),
            buf,
            sizeof buf
        );
        if (!res)
            return false;
    }

    return true;
}


CodePatch::CodePatch(DWORD patchAddr, DWORD tgtAddr, GameVersion ver)
{
    codePatches().emplace_back(patchAddr, tgtAddr, ver);
}


extern "C" bool is_1_0()
{
    return *detectVersion() == GameVersion::Version1_0;
}

extern "C" bool is_1_6()
{
    return *detectVersion() == GameVersion::Version1_6;
}
