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

            if (target == "..") {
                shell->upPath();
            } else {
                shell->goToPath(target);
            }
        }
    }
}
