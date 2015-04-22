#include <iostream>
#include "Shell.h"
#include "HelpCommand.h"

namespace RhIO
{
    std::string HelpCommand::getName()
    {
        return "help";
    }

    std::string HelpCommand::getDesc()
    {
        return "Lists all commands";
    }

    void HelpCommand::process(std::vector<std::string> args)
    {
        auto commands = shell->getCommands();

        for (auto entry : commands) {
            auto command = entry.second;
            Terminal::setColor("white", true);
            std::cout << command->getName() << ": " << std::endl;
            Terminal::clear();
            if (command->getUsage() != "") {
                std::cout << "    Usage: " << command->getUsage() << std::endl;
            }
            std::cout << "    " << command->getDesc() << std::endl;
        }
    }
}
