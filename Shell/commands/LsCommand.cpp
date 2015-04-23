#include <sstream>
#include <iostream>
#include "Shell.h"
#include "LsCommand.h"
#include "Node.h"
#include "NodePool.h"

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
        
        Node *node = shell->getNode(dir);
        if (node == NULL) {
            std::stringstream ss;
            ss << "Unable to get node " << dir;
            throw ss.str();
        } else {
            // Listing sub directories
            Terminal::setColor("blue", true);
            for (auto child : node->children) {
                std::cout << child.first;
                std::cout << "/" << std::endl;
            }
            Terminal::clear();

            // Listing commands
            /*
            for (auto command : node->getCommands()) {
                Terminal::setColor("green", true);
                printf("%-23s", command.c_str());
            
                if (auto shellCommand = shell->getCommand(command)) {
                    Terminal::setColor("grey", false);
                    std::cout << "desc: ";
                    Terminal::clear();
                    std::cout << shellCommand->getDesc();
                }
                std::cout << std::endl;
                Terminal::clear();
            }
            */

            NodePool pool = shell->poolForNode(node);
            pool.draw();
        }
    }
}
