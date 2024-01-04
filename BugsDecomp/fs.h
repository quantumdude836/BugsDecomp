
// Basic filesystem wrappers.

#pragma once


/// <summary>
/// Opens a file descriptor for the given path. If it fails, retries with the
/// path relative to the datas root.
/// 
/// NOTE: the path should start with "..\", as the retry strips the first 3
/// characters from the path.
/// </summary>
/// <param name="path">Path to open</param>
/// <param name="oflag">Open flags</param>
/// <returns>File descriptor, or -1</returns>
EXTERN_C int FsOpen(const char *path, int oflag, ...);
PATCH_CODE(0x4056c0, 0x4056c0, FsOpen);

/// <summary>
/// Opens a file object for the given path. If it fails, retries with the path
/// relative to the datas root.
/// 
/// NOTE: the path should start with "..\", as the retry strips the first 3
/// characters from the path.
/// </summary>
/// <param name="path">Path to open</param>
/// <param name="mode">Open mode</param>
/// <returns>File object, or null</returns>
EXTERN_C FILE *FsFOpen(const char *path, const char *mode);
PATCH_CODE(0x405760, 0x405760, FsFOpen);

// wrapper around _lseek
EXTERN_C long FsSeek(int fd, long offset, int whence);
PATCH_CODE(0x405800, 0x405800, FsSeek);

// wrapper around _read
EXTERN_C int FsRead(int fd, void *buf, unsigned n);
PATCH_CODE(0x405820, 0x405820, FsRead);

// wrapper around _close
EXTERN_C int FsClose(int fd);
PATCH_CODE(0x405840, 0x405840, FsClose);

/// <summary>
/// Searches system drives for the datas root directory.
/// </summary>
/// <returns>Whether the datas root was found</returns>
EXTERN_C BOOL FsFindRoot(void);
PATCH_CODE(0x405850, 0x405850, FsFindRoot);
