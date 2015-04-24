#pragma once

#include <iostream>
#include "Command.h"
#include <Value.hpp>

namespace RhIO
{
    class GnuPlot;
    class NodePool;
    class PlotCommand : public Command
    {
        public:
            virtual std::string getName();
            virtual std::string getDesc();
            virtual void process(std::vector<std::string> args);

            void update(GnuPlot *plot, NodePool *pool);

        protected:
            bool paused;
    };
}
