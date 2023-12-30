
Decompilation project for the PC game "Bugs Bunny: Lost in Time!" (aka BBLiT)
by Behaviour Interactive.

The decompiled code is built into a DLL; a custom launcher starts the game and
injects the DLL into the game's process, allowing the decompiled functions to be
patched over their original versions without modifying the binary on disk.

# Solution Layout

The VS solution consists of the following projects:
- [BugsDecomp](#decomp) - DLL containing decompiled code
- [Launcher](#launcher) - CLI game launcher

## BugsDecomp {#decomp}

DLL project which contains all the decompiled code for the game. Will attempt to
detect the version of the game, allowing it to be used in both the 1.0 and 1.6
versions.

In order to register functions to be patched, the header file `CodePatch.h`
provides the macros `PATCH_CODE_1_0` and `PATCH_CODE_1_6` to register patches
for version 1.0 and 1.6, respectively. These macros MUST be used at the global
scope to be registered/applied properly!

## Launcher {#launcher}

A simple command-line utility for starting the game and injecting the
decompilation DLL.

Launcher options:
- `/help` - shows launcher help and exits
- `/exe:<exe path>` - sets the path to the game to launch
- `/cmdline:<cmdline>` - specifies command line for the game
- `/dll:<dll path>` - set path to the DLL to inject

Currently, the `/exe:` option is required; in the future, the launcher will have
the ability to check the registry for an installed copy of BBLiT.

The default command line is `/b00 /win /x1024 /y768 /opengl`, which starts the
game in windowed mode, 1024x768, with the OpenGL renderer. More information
about game options can be found [here](doc\internals.md#options).

The `/dll:` option is required; while the launcher could be coded to launch the
game without DLL injection, it'd be just as easy to use the vanilla launcher.
