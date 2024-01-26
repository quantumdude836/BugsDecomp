
// C runtime functions in the game's code.

#pragma once


#define malloc_bugs (( \
    void *(*)(size_t) \
)VER(0x44fdd0, 0x44fc30))

#define free_bugs (( \
    void (*)(void *) \
)VER(0x44ff10, 0x44fd70))

#define _lseek_bugs (( \
    long (*)(int, long, int) \
)VER(0x44ff60, 0x44fdc0))

#define _read_bugs (( \
    int (*)(int, void *, unsigned) \
)VER(0x450020, 0x44fe80))

#define fseek_bugs (( \
    int (*)(FILE *, long, int) \
)VER(0x450370, 0x4501d0))

#define ftell_bugs (( \
    long (*)(FILE *) \
)VER(0x450410, 0x450270))

#define fclose_bugs (( \
    int (*)(FILE *) \
)VER(0x4506a0, 0x450500))

#define fwrite_bugs (( \
    size_t (*)(const void *, size_t, size_t, FILE *) \
)VER(0x450760, 0x4505c0))

#define fopen_bugs (( \
    FILE *(*)(const char *, const char *) \
)VER(0x4508e0, 0x450740))

#define rand_bugs (( \
    int (*)(void) \
)VER(0x4509b0, 0x450810))

#define fread_bugs (( \
    size_t (*)(void *, size_t, size_t, FILE *) \
)VER(0x450a90, 0x4508f0))

#define _close_bugs (( \
    int (*)(int) \
)VER(0x4513d0, 0x451230))

#define _open_bugs (( \
    int (*)(const char *, int, ...) \
)VER(0x458620, 0x458480))
