#include <iostream>
#include "Shell.h"
#include "LsCommand.h"
#include "Values.h"

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

        // Listing sub directories
        auto children = shell->getClient()->listChildren(path);
        for (auto child : children) {
            Terminal::setColor("blue", true);
            std::cout << child;
            Terminal::clear();
            std::cout << "/" << std::endl;
        }

        Values *values = shell->getCurrentNode();

        for (auto val : values->getAll()) 
        {
            Terminal::setColor("grey", false);
            printf("%6s ", Values::getType(val).c_str());
            Terminal::clear();
            printf("%-15s", val->name.c_str());
            std::cout << " ";

            if (val->comment != "") {
                Terminal::setColor("grey", false);
                std::cout << " desc: ";
                Terminal::clear();
                std::cout << val->comment;
            }
            std::cout << std::endl;
        }
    }
}
