#pragma once

#include <iostream>
#include "Command.h"
#include <RhIOClient.hpp>

namespace RhIO
{
    class Node;
    class SaveCommand : public Command
    {
        public:
            virtual std::string getName();
            virtual std::string getDesc();
            virtual void process(std::vector<std::string> args);
    };
}
