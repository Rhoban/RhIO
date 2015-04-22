#include <iostream>
#include "Shell.h"
#include "LsCommand.h"
#include "Node.h"

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
        std::string dir = "";

        if (args.size()) {
            dir = args[0];
        }
        
        Node *values = shell->getNode(dir);
        if (values == NULL) {
            Terminal::setColor("red", true);
            std::cout << "Unable to get node " << dir << std::endl;
            Terminal::clear();
        } else {
            // Listing sub directories
            for (auto child : values->children) {
                Terminal::setColor("blue", true);
                std::cout << child.first;
                std::cout << "/" << std::endl;
                Terminal::clear();
            }

            for (auto nodeVal : values->getAll()) 
            {
                auto val = nodeVal.value;

                printf("%-15s", val->name.c_str());
                std::cout << " ";
                
                Terminal::setColor("grey", false);
                printf("%6s ", Node::getType(val).c_str());
                Terminal::clear();

                Terminal::setColor("grey", false);
                std::cout << " val: ";
                Terminal::clear();
                Node::get(shell, nodeVal);
                printf("%-8s", Node::toString(val).c_str());

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
}
