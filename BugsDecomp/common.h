
// Common header for decompiled code.

#pragma once


#include "CodePatch.h"
#include "GameVersion.h"


// helper macro for referencing a string located in game memory; the string
// param is only for documentation
#define BSTR(addr, str) (reinterpret_cast<const char *>(addr))
// ditto for floats
#define BFLT(addr, val) (*reinterpret_cast<const float *>(addr))
#define BDBL(addr, val) (*reinterpret_cast<const double *>(addr))
