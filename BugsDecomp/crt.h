
// C runtime functions in the game's code.

#pragma once


#define fopen_bugs (reinterpret_cast<FILE *(*)(const char *, const char *)>( \
    VER(0x4508e0, 0x450740)))
#define _open_bugs (reinterpret_cast<int (*)(const char *, int, ...)>( \
    VER(0x458620, 0x458480)))
