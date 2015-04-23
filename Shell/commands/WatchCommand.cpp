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
            pool = shell->poolForNode(shell->getCurrentNode());
        } else {
            pool = shell->getPool(args);
        }

        pool.setCallback(std::bind(&WatchCommand::update, this, _1));
    
        system("clear");
        pool.draw();

        shell->streamWait(&pool);
    }

    void WatchCommand::update(NodePool *pool)
    {
        system("clear");
        pool->draw();
    }
}