#include <iostream>
#include "Command.h"
#include "Terminal.h"

namespace RhIO
{
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
        Terminal::setColor("red", true);
        std::cout << "Error, usage: " << getUsage() << std::endl;
        Terminal::clear();
    }
}
