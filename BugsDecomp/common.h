
// Common header for decompiled code.

#pragma once


#include "CodePatch.h"
#include "GameVersion.h"


#ifdef __cplusplus

// helper macro for referencing a string located in game memory; the string
// param is only for documentation
#define BSTR(addr, str) (reinterpret_cast<const char *>(addr))
// ditto for floats
#define BFLT(addr, val) (*reinterpret_cast<const float *>(addr))
#define BDBL(addr, val) (*reinterpret_cast<const double *>(addr))

#else

// same as above, but C syntax
#define BSTR(addr, str) ((const char *)(addr))
#define BFLT(addr, val) (*(const float *)(addr))
#define BDBL(addr, val) (*(const double *)(addr))

#endif
