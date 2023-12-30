
// C runtime functions in the game's code.

#pragma once


#define _lseek_bugs (reinterpret_cast< \
    long (*)(int, long, int) \
>(VER(0x44ff60, 0x44fdc0)))

#define _read_bugs (reinterpret_cast< \
    int (*)(int, void *, unsigned) \
>(VER(0x450020, 0x44fe80)))

#define fopen_bugs (reinterpret_cast< \
    FILE *(*)(const char *, const char *) \
>(VER(0x4508e0, 0x450740)))

#define _close_bugs (reinterpret_cast< \
    int (*)(int) \
>(VER(0x4513d0, 0x451230)))

#define _open_bugs (reinterpret_cast< \
    int (*)(const char *, int, ...) \
>(VER(0x458620, 0x458480)))
