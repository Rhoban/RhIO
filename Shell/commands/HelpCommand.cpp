#include <iostream>
#include "Shell.h"
#include "HelpCommand.h"
#include "RemoteCommand.h"

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
            bool isRemote = (NULL!=dynamic_cast<RemoteCommand*>(command));

            Terminal::setColor(isRemote ? "green" : "white", true);
            std::cout << command->getName() << ": ";
            Terminal::clear();
            
            if (auto remote = dynamic_cast<RemoteCommand*>(command)) {
                std::cout << "(from ";
                Terminal::setColor("grey", true);
                std::cout << "/" << remote->getOrigin() << "/";
                Terminal::clear();
                std::cout << ")";
            }

            std::cout << std::endl;
            if (command->getUsage() != "") {
                std::cout << "    Usage: " << command->getUsage() << std::endl;
            }
            std::cout << "    " << command->getDesc() << std::endl;
        }
    }
}
