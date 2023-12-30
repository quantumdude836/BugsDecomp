
// Basic filesystem wrappers.

#pragma once


#define datasRootDir (reinterpret_cast<char *>(0x4b1928))


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
extern "C" int FsOpen(const char *path, int oflag, ...);

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
extern "C" FILE *FsFOpen(const char *path, const char *mode);
