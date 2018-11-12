#pragma once

#include <iostream>
#include "Command.h"
#include <rhio_client/RhIOClient.hpp>

namespace RhIO
{
    class NodePool;
    class WatchCommand : public Command
    {
        public:
            virtual std::string getName();
            virtual std::string getDesc();
            virtual void process(std::vector<std::string> args);

            void update(NodePool *pool);
    };
}
