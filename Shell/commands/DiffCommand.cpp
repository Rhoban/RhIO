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
        std::string dir = "";

        if (args.size()) {
            dir = args[0];
        }
        
        Node *node = shell->getNode(dir);
        
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
                diff++;
                Terminal::setColor("white", true);
                std::cout << nodeVal.getName();
                Terminal::setColor("white", false);
                std::cout << ": ";
                Terminal::setColor("red", true);
                std::cout << Node::persistedToString(value);
                Terminal::setColor("white", false);
                std::cout << " -> ";
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
