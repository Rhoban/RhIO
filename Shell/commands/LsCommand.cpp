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

            NodePool pool;
            for (auto nodeVal : values->getAll()) {
                Node::get(shell, nodeVal);
                pool.push_back(nodeVal);
            }
            pool.draw();
        }
    }
}
