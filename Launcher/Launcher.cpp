
#include "stdafx.h"
#include "Launcher.h"


Launcher::Launcher()
{
}


void Launcher::showUsage()
{
    puts("Launcher options:");
    puts("    /help           Show this help and exit");
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
    }

    return std::nullopt;
}


int Launcher::run(int argc, char **argv)
{
    assert(argc > 0);
    assert(argv);
    // argv[0] goes to prog name; rest go to args
    std::string progName(argv[0]);
    std::vector<const char *> args(&argv[1], &argv[argc]);

    // parse args
    if (auto res = parseArgs(std::move(args)))
        return *res;

    puts("What are you doing here? There's nothing implemented yet!");
    return 0;
}
