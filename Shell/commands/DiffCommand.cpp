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
        return "Shows the diff from the current node";
    }

    void DiffCommand::process(std::vector<std::string> args)
    {
        Node *node = shell->getNode();
        
        if (!showDiff(node)) {
            std::cout << "Everything is clean" << std::endl;
        }
    }

    int DiffCommand::showDiff(Node *node)
    {
        int diff = 0;

        for (auto nodeVal : node->getAll()) {
            auto value = nodeVal.value;
            if (value->persisted && Node::isDiff(value)) {
                diff++;
                Terminal::setColor("white", true);
                std::cout << nodeVal.getName();
                Terminal::setColor("white", false);
                std::cout << " differs from ";
                Terminal::setColor("red", true);
                std::cout << Node::persistedToString(value);
                Terminal::setColor("white", false);
                std::cout << " to ";
                Terminal::setColor("green", true);
                std::cout << Node::toString(value);
                std::cout << std::endl;
            }
        }

        for (auto entry : node->children) {
            diff += showDiff(entry.second);
        }
        return diff;
    }
}
