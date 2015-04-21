#include "Command.h"

namespace Rhio
{
    void Command::setShell(Shell *shell_)
    {
        shell = shell_;
    }

    std::string Command::getUsage()
    {
        return "";
    }
}
