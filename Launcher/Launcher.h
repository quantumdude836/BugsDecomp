
// Class for main launcher logic.

#pragma once


class Launcher
{
public:
    /// <summary>
    /// Creates a new launcher instance.
    /// </summary>
    Launcher();

public:
    /// <summary>
    /// Runs the launcher.
    /// </summary>
    /// <param name="argc">`argc` from program entry</param>
    /// <param name="argv">`argv` from program entry</param>
    /// <returns>Program return code</returns>
    int run(int argc, char **argv);
};
