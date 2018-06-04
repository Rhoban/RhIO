#include <sstream>
#include <iostream>
#include "Shell.h"
#include "LoadCommand.h"
#include <Node.h>
#include <NodePool.h>

namespace RhIO
{
    std::string LoadCommand::getName()
    {
        return "load";
    }

    std::string LoadCommand::getDesc()
    {
        return "Load the values";
    }

    void LoadCommand::process(std::vector<std::string> args)
    {
        //Retrieve config path
        std::string target = "rhio";
        if (auto value = shell->getValue("/server/config")) {
            target = Node::toString(value);
        } 
        
        auto node = getNode(args);
        auto name = node->getPath();
        if (name != "") {
            target += "/" + name;
        }

        shell->getClient()->load(node->getPath(), target);
        shell->sync();
    }
}
