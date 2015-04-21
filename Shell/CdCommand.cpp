#include <iostream>
#include "Shell.h"
#include "CdCommand.h"

namespace RhIO
{
    std::string CdCommand::getName()
    {
        return "cd";
    }

    std::string CdCommand::getDesc()
    {
        return "Changing the directory";
    }

    std::string CdCommand::getUsage()
    {
        return "cd [path]";
    }

    void CdCommand::process(std::vector<std::string> args)
    {
        if (args.size() != 1) {
            errorUsage();
        } else {
            auto target = args.front();

            if (target == "/") {
                shell->rootPath();
            } else if (target == "..") {
                shell->upPath();
            } else {
                auto path = shell->getPath();
                auto children = shell->getClient()->listChildren(path);
                for (auto child : children) {
                    if (target == child) {
                        shell->enterPath(child);
                        return;
                    }
                }
                Terminal::setColor("red", true);
                std::cout << "No such directory: " << target << std::endl;
                Terminal::clear();
            }
        }
    }
}
