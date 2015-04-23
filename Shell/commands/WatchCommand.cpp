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
        NodePool pool;
        if (args.size() == 0) {
            Node *values = shell->getNode();
            for (auto nodeVal : values->getAll()) {
                Node::get(shell, nodeVal);
                pool.push_back(nodeVal);
            }
        } else {
            for (int k=0; k<args.size(); k++) {
                auto val = shell->getNodeValue(args[k]);
                if (val.value == NULL) {
                    Terminal::setColor("red", true);
                    std::cout << "Unknown parameter: " << args[k] << std::endl;
                    Terminal::clear();
                    return;
                }
                pool.push_back(val);
            }
        }


        pool.setCallback(std::bind(&WatchCommand::update, this, _1));
    
        system("clear");
        pool.draw();

        shell->getStream()->addPool(&pool);
        std::string line;
        std::getline(std::cin, line);
        shell->getStream()->removePool(&pool);
    }

    void WatchCommand::update(NodePool *pool)
    {
        system("clear");
        pool->draw();
    }
}
