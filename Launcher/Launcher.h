
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

private:
    /// <summary>
    /// Starts the game process.
    /// </summary>
    /// <param name="procInfo">Output process information</param>
    /// <returns>Whether the launch succeeded</returns>
    bool launchGame(PROCESS_INFORMATION &procInfo) const;

private:
    /// <summary>
    /// Gets the full path to the DLL.
    /// </summary>
    /// <returns>Full path to the DLL</returns>
    std::string getFullDllPath() const;
    /// <summary>
    /// Injects the DLL into the process.
    /// </summary>
    /// <param name="hProcess">Handle to the game process</param>
    /// <returns>Whether the inject succeeded</returns>
    bool injectDll(HANDLE hProcess) const;

public:
    /// <summary>
    /// Runs the launcher.
    /// </summary>
    /// <param name="argc">`argc` from program entry</param>
    /// <param name="argv">`argv` from program entry</param>
    /// <returns>Program return code</returns>
    int run(int argc, char **argv);

private:
    // path to the game to launch
    std::string exePath;
    // working directory for the game
    std::string cwdPath;
    // command line for game process
    std::string cmdline;
    // path to the injected DLL
    std::string dllPath;
    // whether to pause before DLL injection
    bool pause;
};
