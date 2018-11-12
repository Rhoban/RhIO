#pragma once

#include <iostream>
#include "Command.h"
#include <rhio_client/RhIOClient.hpp>

namespace RhIO
{
    class GnuPlot;
    class NodePool;
    class Plot3DCommand : public Command
    {
        public:
            virtual std::string getName();
            virtual std::string getDesc();
            std::string getUsage();
            virtual void process(std::vector<std::string> args);

            void update(GnuPlot *plot, NodePool *pool);

        protected:
            bool paused;
    };
}
