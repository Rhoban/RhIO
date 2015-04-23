#include <sstream>
#include <iostream>
#include "Command.h"
#include "Terminal.h"

namespace RhIO
{
    Command::~Command()
    {
    }

    void Command::setShell(Shell *shell_)
    {
        shell = shell_;
    }

    std::string Command::getUsage()
    {
        return "";
    }
            
    void Command::errorUsage()
    {
        std::stringstream ss;
        ss << "Usage: " << getUsage();
        throw ss.str();
    }
}
