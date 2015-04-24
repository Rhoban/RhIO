#include <signal.h>
#include "commands/HelpCommand.h"
#include "commands/LsCommand.h"
#include "commands/WatchCommand.h"
#include "commands/CdCommand.h"
#include "commands/ClearCommand.h"
#include "commands/LogCommand.h"
#include "commands/SyncCommand.h"
#include "commands/PlotCommand.h"
#include "commands/DiffCommand.h"
#include "commands/LoadCommand.h"
#include "commands/SaveCommand.h"
#include "commands/TreeCommand.h"
#ifdef HAS_CURSES
#include "commands/TuneCommand.h"
#endif
#include "Shell.h"

using namespace RhIO;

Shell *shell = NULL;

void shell_quit(int s)
{
    if (shell != NULL) {
        shell->quit();
    }
    exit(1);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, shell_quit);

    std::string server = "localhost";

    if (argc > 1) {
        server = std::string(argv[1]);
    }

    shell = new Shell(server);
    shell->registerCommand(new HelpCommand);
    shell->registerCommand(new LsCommand);
    shell->registerCommand(new CdCommand);
    shell->registerCommand(new ClearCommand);
    shell->registerCommand(new WatchCommand);
    shell->registerCommand(new LogCommand);
    shell->registerCommand(new SyncCommand);
    shell->registerCommand(new PlotCommand);
    shell->registerCommand(new DiffCommand);
    shell->registerCommand(new LoadCommand);
    shell->registerCommand(new SaveCommand);
    shell->registerCommand(new TreeCommand);
#ifdef HAS_CURSES
    shell->registerCommand(new TuneCommand);
#endif

    shell->addAlias("l", "ls");
    shell->addAlias("ll", "ls");

    shell->run();
}
