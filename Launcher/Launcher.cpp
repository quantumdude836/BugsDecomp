
#include "stdafx.h"
#include "Launcher.h"


Launcher::Launcher()
{
}


int Launcher::run(int argc, char **argv)
{
    assert(argc > 0);
    assert(argv);
    // argv[0] goes to prog name; rest go to args
    std::string progName(argv[0]);
    std::vector<std::string> args(&argv[1], &argv[argc]);

    puts("What are you doing here? There's nothing implemented yet!");
    return 0;
}
