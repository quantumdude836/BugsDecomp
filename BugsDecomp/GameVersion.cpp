
#include "stdafx.h"
#include "GameVersion.h"


// helper for accessing a possibly-invalid address
bool tryReadMem(void *dst, const void *src, size_t size)
{
    SIZE_T numRead;

    BOOL res = ReadProcessMemory(
        GetCurrentProcess(),
        src,
        dst,
        size,
        &numRead
    );
    if (!res || numRead != size)
        return false;

    return true;
}


// actual version detection code
// FIXME: cleanup/make more robust
std::optional<GameVersion> detectVersionImpl()
{
    UINT64 buf;

    if (!tryReadMem(&buf, reinterpret_cast<const void *>(0x45ca00), 8))
        return std::nullopt;

    if (buf == 0x3fe2d2d2d2d2d2d3ull)
        return GameVersion::Version1_0;
    if (buf == 0x3fe5555555555555ull)
        return GameVersion::Version1_6;

    return std::nullopt;
}

std::optional<GameVersion> detectVersion()
{
    // outer optional indicates whether detection has been run/cached; inner
    // optional is whether detected version is valid
    static std::optional<std::optional<GameVersion>> cachedVer = std::nullopt;

    if (!cachedVer)
        cachedVer = detectVersionImpl();

    return *cachedVer;
}
