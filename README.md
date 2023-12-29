
Decompilation project for the PC game "Bugs Bunny: Lost in Time!" (aka BBLiT)
by Behaviour Interactive.

The VS solution consists of the following projects:
- [Launcher](#launcher) - CLI game launcher

# Launcher {#launcher}

A simple command-line utility for starting the game. Will eventually be used to
inject the DLL containing decompiled code.

Launcher options:
- `/help` - shows launcher help and exits
- `/path:<root path>` - sets the root path to a BBLiT copy
- `/cmdline:<cmdline>` - specifies command line for the game

Currently, the `/path:` option is required; in the future, the launcher will
have the ability to check the registry for an installed copy of BBLiT.

The default command line is `/b00 /win /x1024 /y768 /opengl`, which starts the
game in windowed mode, 1024x768, with the OpenGL renderer.
