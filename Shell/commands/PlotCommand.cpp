#include <iostream>
#include "Shell.h"
#include "PlotCommand.h"
#include "Node.h"
#include "NodePool.h"
#include "Stream.h"
#include <GnuPlot.h>

using namespace std::placeholders;

namespace RhIO
{
    std::string PlotCommand::getName()
    {
        return "plot";
    }

    std::string PlotCommand::getDesc()
    {
        return "Plot parameters";
    }

    void PlotCommand::process(std::vector<std::string> args)
    {
        NodePool pool;
        if (args.size() == 0) {
            pool = shell->poolForNode(shell->getCurrentNode());
        } else {
            pool = shell->getPool(args);
        }

        GnuPlot plot;
        pool.setCallback(std::bind(&PlotCommand::update, this, &plot, _1));

        shell->streamWait(&pool);
        plot.closeWindow();
    }

    void PlotCommand::update(GnuPlot *plot, NodePool *pool)
    {
        plot->setX(pool->timestamp);
        for (auto node : *pool) {
            plot->push(node.value->name, Node::toNumber(node.value));
        }
        plot->render();
    }
}
