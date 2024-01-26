
Decompilation project for the PC game "Bugs Bunny: Lost in Time!" (aka BBLiT)
by Behaviour Interactive. Specifically, it supports the two known versions:

- 1.0 (Bugs.exe MD5 = 59731F0354F8A66E63502BD3B521BD82)
- 1.6 (Bugs.exe MD5 = 121DB63D76ED5077D69837B111163CAC)

The decompiled code is built into a DLL; a custom launcher starts the game and
injects the DLL into the game's process, allowing the decompiled functions to be
patched over their original versions without modifying the binary on disk.

# Solution Layout

The VS solution consists of the following projects:

- [BugsDecomp](#bugsdecomp) - DLL containing decompiled code
- [Launcher](#launcher) - CLI game launcher

## BugsDecomp

DLL project which contains all the decompiled code for the game. Will attempt to
detect the version of the game, allowing it to be used in both the 1.0 and 1.6
versions.

In order to register functions to be patched, the header file `CodePatch.h`
provides the macros `PATCH_CODE_1_0` and `PATCH_CODE_1_6` to register patches
for version 1.0 and 1.6, respectively. These macros MUST be used at the global
scope to be registered/applied properly! For convenience, there's also a
`PATCH_CODE` macro for registering the same function for both versions (but with
potentially different addresses).

Decompiled functions do not follow any sort of strict naming conventions, as
there's no knowledge of the original function names (other than a macro or
inline function named `getMem`). That being said, the following are used as
guidelines:

- Names specific to a version should be suffixed with `_major_minor` (e.g.
`_1_6`)
- Names that clash with existing declarations (such as C runtime functions)
should be suffixed with `_bugs`

When decompiling code, any references to C runtime functions that use or modify
global state MUST refer to the existing versions in the game! This is to ensure
the global state is kept consistent between old and new code.

## Launcher

A simple command-line utility for starting the game and injecting the
decompilation DLL.

Launcher options:

- `/help` - shows launcher help and exits
- `/exe:<exe path>` - sets the path to the game to launch
- `/cwd:<work dir>` - sets the working directory for the game
- `/cmdline:<cmdline>` - specifies command line for the game
- `/dll:<dll path>` - set path to the DLL to inject
- `/pause` - pauses launcher before DLL injection

Currently, the `/exe:` and `/cwd:` options are required; in the future, the
launcher will have the ability to check the registry for an installed copy of
BBLiT to set up defaults.

The default command line is `/b00 /win /x1024 /y768 /opengl`, which starts the
game in windowed mode, 1024x768, with the OpenGL renderer. More information
about game options can be found [here](doc/internals.md#launch-options).

The `/dll:` option is required; while the launcher could be coded to launch the
game without DLL injection, it'd be just as easy to use the vanilla launcher.

The `/pause` option allows a debugger to be attached to the game process before
any code is run (including the DLL's attach code).
