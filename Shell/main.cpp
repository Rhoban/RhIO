#include "commands/HelpCommand.h"
#include "commands/LsCommand.h"
#include "commands/WatchCommand.h"
#include "commands/CdCommand.h"
#include "commands/ClearCommand.h"
#include "commands/LogCommand.h"
#ifdef HAS_CURSES
#include "commands/TuneCommand.h"
#endif
#include "Shell.h"

using namespace RhIO;

int main(int argc, char *argv[])
{
    std::string server = "localhost";

    if (argc > 1) {
        server = std::string(argv[1]);
    }

    Shell shell(server);
    shell.registerCommand(new HelpCommand);
    shell.registerCommand(new LsCommand);
    shell.registerCommand(new CdCommand);
    shell.registerCommand(new ClearCommand);
    shell.registerCommand(new WatchCommand);
    shell.registerCommand(new LogCommand);
#ifdef HAS_CURSES
    shell.registerCommand(new TuneCommand);
#endif
    shell.run();
}
