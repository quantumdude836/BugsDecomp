
// Win32 precompiled header.

#pragma once


// needed for vsprintf
#define _CRT_SECURE_NO_WARNINGS
#define DIRECTINPUT_VERSION 0x0500
#define DIRECTSOUND_VERSION 0x0700

// Windows headers
#include <Windows.h>
#include <mmreg.h>
#include <dinput.h>
#include <dsound.h>

// C headers
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif

// POSIX headers
#include <io.h>
#include <fcntl.h>

// C++ headers
#ifdef __cplusplus
#include <optional>
#include <vector>
#endif


// pi and tau (aka 2*pi)
#define PI 3.1415926535897932384626433832795
#define TAU (2.0 * PI)
