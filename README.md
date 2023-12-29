
Decompilation project for the PC game "Bugs Bunny: Lost in Time!" (aka BBLiT)
by Behaviour Interactive.

# Solution Layout

The VS solution consists of the following projects:
- [BugsDecomp](#decomp) - DLL containing decompiled code
- [Launcher](#launcher) - CLI game launcher

## BugsDecomp {#decomp}

DLL project which contains all the decompiled code for the game.

## Launcher {#launcher}

A simple command-line utility for starting the game. Will eventually be used to
inject the DLL containing decompiled code.

Launcher options:
- `/help` - shows launcher help and exits
- `/path:<root path>` - sets the root path to a BBLiT copy
- `/cmdline:<cmdline>` - specifies command line for the game

Currently, the `/path:` option is required; in the future, the launcher will
have the ability to check the registry for an installed copy of BBLiT.

The default command line is `/b00 /win /x1024 /y768 /opengl`, which starts the
game in windowed mode, 1024x768, with the OpenGL renderer. More information
about game options can be found [here](doc\internals.md#options).
