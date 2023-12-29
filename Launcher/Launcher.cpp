
#include "stdafx.h"
#include "Launcher.h"


/// <summary>
/// Checks an argument string for a prefix, and returns the suffix on match.
/// </summary>
/// <param name="arg">Argument string to check</param>
/// <param name="prefix">Prefix to check for</param>
/// <returns>
/// Pointer within the arg to the suffix, or `nullptr` for no match
/// </returns>
static const char *checkArgPrefix(const char *arg, const char *prefix)
{
    size_t n = strlen(prefix);
    if (!strncmp(arg, prefix, n))
        return arg + n;

    return nullptr;
}


Launcher::Launcher() :
    cmdline(" /b00 /win /x1024 /y768 /opengl")
{
}


void Launcher::showUsage()
{
    puts("Launcher options:");
    puts("    /help               Show this help and exit");
    puts("    /path:<bblit path>  Set BBLiT root path");
    puts("    /cmdline:<cmdline>  Specify command line for the game");
    puts("    /dll:<dll path>     Set path to the DLL to inject");
}

std::optional<int> Launcher::parseArgs(std::vector<const char *> &&args)
{
    for (auto arg : args)
    {
        // only allow non-empty args that start with forward slash
        if (arg[0] != '/')
        {
            fprintf(stderr, "Invalid argument '%s'\n", arg);
            return 1;
        }

        if (!strcmp(arg, "/help"))
        {
            showUsage();
            return 0;
        }

        if (const char *val = checkArgPrefix(arg, "/path:"))
        {
            rootPath = val;
            continue;
        }

        if (const char *val = checkArgPrefix(arg, "/cmdline:"))
        {
            cmdline = val;
            continue;
        }

        if (const char *val = checkArgPrefix(arg, "/dll:"))
        {
            dllPath = val;
            continue;
        }

        fprintf(stderr, "Invalid argument '%s'\n", arg);
        return 1;
    }

    return std::nullopt;
}


std::string Launcher::getGameExePath() const
{
    // for now, assume root path has no trailing slashes
    return rootPath + "\\bin\\Bugs.exe";
}

bool Launcher::launchGame(PROCESS_INFORMATION &procInfo) const
{
    std::string exePath = getGameExePath();

    STARTUPINFO startInfo = { 0 };
    startInfo.cb = sizeof startInfo;

    // start the game!
    BOOL res = CreateProcessA(
        exePath.c_str(),
        const_cast<char *>(cmdline.c_str()),
        nullptr,
        nullptr,
        FALSE,
        CREATE_SUSPENDED,
        nullptr,
        nullptr,
        &startInfo,
        &procInfo
    );
    if (!res)
        return false;

    return true;
}


int Launcher::run(int argc, char **argv)
{
    PROCESS_INFORMATION procInfo;

    assert(argc > 0);
    assert(argv);
    // skip argv[0] (prog name); rest go to args
    std::vector<const char *> args(&argv[1], &argv[argc]);

    // parse args
    if (auto res = parseArgs(std::move(args)))
        return *res;

    // at this point, root path must be set
    if (rootPath.empty())
    {
        fputs("BBLiT root path not specified\n", stderr);
        return 2;
    }
    // DLL path must also be set
    if (dllPath.empty())
    {
        fputs("DLL path not specified\n", stderr);
        return 2;
    }

    // launch game process
    if (!launchGame(procInfo))
    {
        fputs("Unable to start game\n", stderr);
        return 3;
    }

    // game's main thread is ready to run now
    ResumeThread(procInfo.hThread);

    CloseHandle(procInfo.hThread);
    CloseHandle(procInfo.hProcess);
    return 0;
}
