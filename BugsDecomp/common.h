
// Common header for decompiled code.

#pragma once


#include "CodePatch.h"
#include "GameVersion.h"


// helper macro for referencing a string located in game memory; the string
// param is only for documentation
#define BSTR(addr, str) ((const char *)(addr))
// ditto for floats
#define BFLT(addr, val) (*(const float *)(addr))
#define BDBL(addr, val) (*(const double *)(addr))
