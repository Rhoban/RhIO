#include <iostream>
#include "Shell.h"
#include "LsCommand.h"

namespace RhIO
{
    std::string LsCommand::getName()
    {
        return "ls";
    }

    std::string LsCommand::getDesc()
    {
        return "Lists the remote entries";
    }

    void LsCommand::process(std::vector<std::string> args)
    {
        auto path = shell->getPath();

        Terminal::setColor("blue", true);
        auto children = shell->getClient()->listChildren(path);
        for (auto child : children) {
            std::cout << child << std::endl;

        }
        Terminal::clear();
    }
}
