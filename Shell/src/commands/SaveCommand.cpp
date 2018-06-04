#include <sstream>
#include <iostream>
#include "Shell.h"
#include "SaveCommand.h"
#include <Node.h>
#include <NodePool.h>

namespace RhIO
{
    std::string SaveCommand::getName()
    {
        return "save";
    }

    std::string SaveCommand::getDesc()
    {
        return "Save (persist) the values";
    }

    void SaveCommand::process(std::vector<std::string> args)
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

        shell->getClient()->save(node->getPath(), target);
        shell->sync();
    }
}
