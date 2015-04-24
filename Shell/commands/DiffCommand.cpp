#include <sstream>
#include <iostream>
#include "Shell.h"
#include "DiffCommand.h"
#include <Node.h>
#include <NodePool.h>

namespace RhIO
{
    std::string DiffCommand::getName()
    {
        return "diff";
    }

    std::string DiffCommand::getDesc()
    {
        return "Shows the diff";
    }

    void DiffCommand::process(std::vector<std::string> args)
    {
        auto node = getNode(args);

        if (!showDiff(node)) {
            Terminal::setColor("green", true);
            std::cout << "Everything is clean" << std::endl;
            Terminal::clear();
        }
    }

    int DiffCommand::showDiff(Node *node)
    {
        int diff = 0;

        for (auto nodeVal : node->getAll()) {
            auto value = nodeVal.value;
            if (value->persisted && Node::isDiff(value)) {
                Terminal::clear();
                diff++;
                std::string name = std::string("/") + nodeVal.getName() + ":";
                printf("%-20s ", name.c_str());
                
                Terminal::setColor("red", true);
                printf("%-5s", Node::persistedToString(value).c_str());
                Terminal::setColor("white", false);
                std::cout << " → ";
                Terminal::setColor("green", true);
                printf("%-5s", Node::toString(value).c_str());

                std::cout << std::endl;
            }
        }

        for (auto entry : node->children) {
            diff += showDiff(entry.second);
        }
        return diff;
    }
}
