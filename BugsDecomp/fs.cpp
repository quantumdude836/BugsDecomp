
#include "stdafx.h"
#include "common.h"
#include "fs.h"
#include "crt.h"


PATCH_CODE_1_0(0x4056c0, FsOpen);
PATCH_CODE_1_6(0x4056c0, FsOpen);
extern "C" int FsOpen(const char *path, int oflag, ...)
{
    char fullPath[MAX_PATH];

    // ensure files are always opened in binary mode
    oflag |= O_BINARY;

    // try to open with the given path
    int fd = _open_bugs(path, oflag);

    if (fd == -1 && datasRootDir[0])
    {
        // try again relative to datas root
        strcpy(fullPath, datasRootDir);
        strcat(fullPath, path + 3); // skip leading "..\"
        fd = _open_bugs(fullPath, oflag);
    }

    return fd;
}

PATCH_CODE_1_0(0x405760, FsFOpen);
PATCH_CODE_1_6(0x405760, FsFOpen);
extern "C" FILE *FsFOpen(const char *path, const char *mode)
{
    char fullPath[MAX_PATH];

    // try to open with the given path
    FILE *fp = fopen_bugs(path, mode);

    if (!fp && datasRootDir[0])
    {
        // try again relative to datas root
        strcpy(fullPath, datasRootDir);
        strcat(fullPath, path + 3); // skip leading "..\"
        fp = fopen_bugs(fullPath, mode);
    }

    return fp;
}
