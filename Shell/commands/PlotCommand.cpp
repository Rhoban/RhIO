#include <iostream>
#include "Shell.h"
#include "PlotCommand.h"
#include "Node.h"
#include "NodePool.h"
#include "StreamManager.h"
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
        pool = shell->getPool(args);

        Terminal::setColor("white", true);
        std::cout << "Plotting." << std::endl;
        std::cout << "  q: quit" << std::endl;
        std::cout << "  h: change history" << std::endl;
        std::cout << "  p: pause/unpause" << std::endl;
        Terminal::clear();

        paused = false;
        GnuPlot plot;
        pool.setCallback(std::bind(&PlotCommand::update, this, &plot, _1));

        auto stream = shell->getStream();
        stream->addPool(shell, &pool);
        while (true) {
            char c;
            if ((c=getchar())>0) {
                if (c == 'q') {
                    break;
                }
                if (c == 'h') {
                    plot.changeHistory();
                }
                if (c == 'p') {
                    paused = !paused;
                    if (paused) {
                        printf("Paused\n");
                    } else {
                        printf("Unpaused\n");
                    }
                }
            }
        }
        stream->removePool(shell, &pool);

        plot.closeWindow();
    }

    void PlotCommand::update(GnuPlot *plot, NodePool *pool)
    {
        if (!paused) {
            plot->setX(pool->timestamp);
            for (auto node : *pool) {
                plot->push(node.getName(), Node::toNumber(node.value));
            }
            plot->render();
        }
    }
}
