#include <signal.h>
#include "commands/HelpCommand.h"
#include "commands/LsCommand.h"
#include "commands/WatchCommand.h"
#include "commands/CdCommand.h"
#include "commands/ClearCommand.h"
#include "commands/LogCommand.h"
#include "commands/SyncCommand.h"
#include "commands/PlotCommand.h"
#include "commands/Plot2DCommand.h"
#include "commands/Plot3DCommand.h"
#include "commands/DiffCommand.h"
#include "commands/LoadCommand.h"
#include "commands/SaveCommand.h"
#include "commands/TreeCommand.h"
#include "commands/CatCommand.h"
#include "commands/ViewCommand.h"
#ifdef HAS_LOGIMG
#include "commands/LogImgCommand.h"
#endif
#include "commands/RepeatCommand.h"
#include "commands/DelayCommand.h"
#include "commands/PadCommand.h"
#ifdef HAS_CURSES
#include "commands/TuneCommand.h"
#endif
#include "Shell.h"

using namespace RhIO;

Shell *shell = NULL;

void shell_quit(int s)
{
    (void) s;
    if (shell != NULL) {
        if (shell->quit()) {
            exit(1);
        }
    }
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
    shell->registerCommand(new Plot2DCommand);
    shell->registerCommand(new Plot3DCommand);
    shell->registerCommand(new DiffCommand);
    shell->registerCommand(new LoadCommand);
    shell->registerCommand(new SaveCommand);
    shell->registerCommand(new TreeCommand);
    shell->registerCommand(new CatCommand);
    shell->registerCommand(new ViewCommand);
#ifdef HAS_LOGIMG
    shell->registerCommand(new LogImgCommand);
#endif
    shell->registerCommand(new RepeatCommand);
    shell->registerCommand(new DelayCommand);
    shell->registerCommand(new PadCommand);
#ifdef HAS_CURSES
    shell->registerCommand(new TuneCommand);
#endif

    shell->addAlias("ll", "ls");
    shell->addAlias("rep", "repeat");
    shell->addAlias("del", "delay");

    if (argc > 2) {
        std::string args = "";
        for (int k=2; k<argc; k++) {
            args += argv[k];
            args += " ";
        }
        shell->run(args);
    } else {
        shell->run();
    }
    shell->quit();
    delete shell;
}
