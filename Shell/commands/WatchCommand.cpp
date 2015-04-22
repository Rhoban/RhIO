#include <iostream>
#include "Shell.h"
#include "WatchCommand.h"
#include "Node.h"
#include "NodePool.h"
#include "Stream.h"

using namespace std::placeholders;

namespace RhIO
{
    std::string WatchCommand::getName()
    {
        return "watch";
    }

    std::string WatchCommand::getDesc()
    {
        return "Watch parameters from the current node";
    }

    void WatchCommand::process(std::vector<std::string> args)
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
            NodePool pool;
            for (auto nodeVal : values->getAll()) {
                Node::get(shell, nodeVal);
                pool.push_back(nodeVal);
            }
            pool.setCallback(std::bind(&WatchCommand::update, this, _1));
        
            system("clear");
            pool.draw();

            shell->getStream()->addPool(&pool);
            std::string line;
            std::getline(std::cin, line);
            shell->getStream()->removePool(&pool);
        }
    }

    void WatchCommand::update(NodePool *pool)
    {
        system("clear");
        pool->draw();
    }
}
