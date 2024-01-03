
// Code for detecting/reporting the running game version.

#pragma once


#ifdef __cplusplus

enum class GameVersion
{
    Version1_0, // 1.0
    Version1_6, // 1.6
};


/// <summary>
/// Detects the version of the running game. Will only run detection code once;
/// if called multiple times, returns the cached result.
/// </summary>
/// <returns>Detected game version, if any</returns>
std::optional<GameVersion> detectVersion();

// wrapper macros for testing against specific versions; only usable if version
// is known to be valid
#define IS_1_0() (*detectVersion() == GameVersion::Version1_0)
#define IS_1_6() (*detectVersion() == GameVersion::Version1_6)

#else

// C wrappers around version detection
bool is_1_0(void);
bool is_1_6(void);

// for convenience, the same macros as in C++
#define IS_1_0() (is_1_0())
#define IS_1_6() (is_1_6())

#endif


// helper macro for picking values depending on version
#define VER(v1_0, v1_6) (IS_1_0() ? (v1_0) : (v1_6))
