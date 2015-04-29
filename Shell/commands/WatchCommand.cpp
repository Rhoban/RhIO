#include <iostream>
#include "Shell.h"
#include "WatchCommand.h"
#include "Node.h"
#include "NodePool.h"
#include "StreamManager.h"

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
            pool = shell->poolForNode(shell->getCurrentNode());
        } else {
            pool = shell->getPool(args);
        }

        pool.setCallback(std::bind(&WatchCommand::update, this, _1));
    
        system("clear");
        pool.draw(true);

        // Reducing frequency when watching values, avoiding shell flickering.
        // We can't see more than 25 fps anyway.
        shell->getStream()->setFrequency(25);
        shell->streamWait(&pool);
    }

    void WatchCommand::update(NodePool *pool)
    {
        system("clear");
        pool->draw(true);
    }
}
