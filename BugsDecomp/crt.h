
// C runtime functions in the game's code.

#pragma once


#define malloc_bugs (reinterpret_cast< \
    void *(*)(size_t) \
>(VER(0x44fdd0, 0x44fc30)))

#define free_bugs (reinterpret_cast< \
    void (*)(void *) \
>(VER(0x44ff10, 0x44fd70)))

#define _lseek_bugs (reinterpret_cast< \
    long (*)(int, long, int) \
>(VER(0x44ff60, 0x44fdc0)))

#define _read_bugs (reinterpret_cast< \
    int (*)(int, void *, unsigned) \
>(VER(0x450020, 0x44fe80)))

#define fseek_bugs (reinterpret_cast< \
    int (*)(FILE *, long, int) \
>(VER(0x450370, 0x4501d0)))

#define ftell_bugs (reinterpret_cast< \
    long (*)(FILE *) \
>(VER(0x450410, 0x450270)))

#define fclose_bugs (reinterpret_cast< \
    int (*)(FILE *) \
>(VER(0x4506a0, 0x450500)))

#define fwrite_bugs (reinterpret_cast< \
    size_t (*)(const void *, size_t, size_t, FILE *) \
>(VER(0x450760, 0x4505c0)))

#define fopen_bugs (reinterpret_cast< \
    FILE *(*)(const char *, const char *) \
>(VER(0x4508e0, 0x450740)))

#define fread_bugs (reinterpret_cast< \
    size_t (*)(void *, size_t, size_t, FILE *) \
>(VER(0x450a90, 0x4508f0)))

#define _close_bugs (reinterpret_cast< \
    int (*)(int) \
>(VER(0x4513d0, 0x451230)))

#define _open_bugs (reinterpret_cast< \
    int (*)(const char *, int, ...) \
>(VER(0x458620, 0x458480)))
