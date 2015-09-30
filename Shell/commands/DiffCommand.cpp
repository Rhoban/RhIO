#include <iomanip>
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
        shell->sync();
        int diff = 0;
        std::cout << std::left;

        for (auto nodeVal : node->getAll()) {
            auto value = nodeVal.value;
            if (value->persisted && Node::isDiff(value)) {
                Terminal::clear();
                diff++;
                std::string name = std::string("/") + nodeVal.getName() + ":";
                std::cout << std::setw(35) << name;
                
                Terminal::setColor("red", true);
                std::cout << std::setw(5) << Node::persistedToString(value);
                Terminal::setColor("white", false);
                std::cout << " → ";
                Terminal::setColor("green", true);
                std::cout << std::setw(5) << Node::toString(value);

                std::cout << std::endl;
            }
        }

        for (auto name : node->getChildren()) {
            diff += showDiff(node->getChild(name));
        }
        return diff;
    }
}
