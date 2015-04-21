#include "HelpCommand.h"
#ifdef HAS_CURSES
#include "TuneCommand.h"
#endif
#include "Shell.h"

using namespace Rhio;

int main()
{
    Shell shell;
    shell.registerCommand(new HelpCommand);
#ifdef HAS_CURSES
    shell.registerCommand(new TuneCommand);
#endif
    shell.run();
}
