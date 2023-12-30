
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

PATCH_CODE_1_0(0x405800, FsSeek);
PATCH_CODE_1_6(0x405800, FsSeek);
extern "C" long FsSeek(int fd, long offset, int whence)
{
    return _lseek_bugs(fd, offset, whence);
}

PATCH_CODE_1_0(0x405820, FsRead);
PATCH_CODE_1_6(0x405820, FsRead);
extern "C" int FsRead(int fd, void *buf, unsigned n)
{
    return _read_bugs(fd, buf, n);
}

PATCH_CODE_1_0(0x405840, FsClose);
PATCH_CODE_1_6(0x405840, FsClose);
extern "C" int FsClose(int fd)
{
    return _close_bugs(fd);
}

PATCH_CODE_1_0(0x405850, FsFindRoot);
PATCH_CODE_1_6(0x405850, FsFindRoot);
extern "C" BOOL FsFindRoot(void)
{
    char rootName[MAX_PATH];
    char volName[MAX_PATH];
    DWORD serNum;
    DWORD maxCompLen;
    DWORD fsFlags;

    UINT oldMode = SetErrorMode(SEM_FAILCRITICALERRORS);
    // search all drives
    strcpy(rootName, BSTR(0x45f368, "a:\\"));
    for (; rootName[0] <= 'z'; rootName[0]++)
    {
        UINT driveType = GetDriveTypeA(rootName);
        // only interested in CD drives
        if (driveType != DRIVE_CDROM)
            continue;

        // look for "BBLIT" volume label
        BOOL res = GetVolumeInformationA(
            rootName,
            volName,
            MAX_PATH,
            &serNum,
            &maxCompLen,
            &fsFlags,
            nullptr,
            0
        );
        if (!res)
            continue;
        if (!_strcmpi(volName, BSTR(0x45f360, "BBLIT")))
        {
            // build datas root from drive letter
            datasRootDir[0] = rootName[0];
            strcpy(&datasRootDir[1], BSTR(0x45f354, ":\\Datas\\"));
            SetErrorMode(oldMode);
            return TRUE;
        }
    }

    // BBLIT CD isn't in the system
    SetErrorMode(oldMode);
    return FALSE;
}
