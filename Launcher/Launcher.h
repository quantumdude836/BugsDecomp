
// Class for main launcher logic.

#pragma once


class Launcher
{
public:
    /// <summary>
    /// Creates a new launcher instance.
    /// </summary>
    Launcher();

private:
    /// <summary>
    /// Prints launcher usage.
    /// </summary>
    void showUsage();
    /// <summary>
    /// Parses program arguments.
    /// </summary>
    /// <param name="args">List of args</param>
    /// <returns>Optional program return code</returns>
    std::optional<int> parseArgs(std::vector<const char *> &&args);

public:
    /// <summary>
    /// Runs the launcher.
    /// </summary>
    /// <param name="argc">`argc` from program entry</param>
    /// <param name="argv">`argv` from program entry</param>
    /// <returns>Program return code</returns>
    int run(int argc, char **argv);
};
