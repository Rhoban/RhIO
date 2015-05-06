#include <stdexcept>
#include <sstream>
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

            if (!shell->goToPath(target)) {
                std::stringstream ss;
                ss << "Unknown node: " << target;
                throw std::runtime_error(ss.str());
            }
        }
    }
}
