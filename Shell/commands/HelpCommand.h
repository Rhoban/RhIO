#pragma once

#include "Command.h"

namespace RhIO
{
    class HelpCommand : public Command
    {
        public:
            virtual std::string getName();
            virtual std::string getDesc();
            virtual void process(std::vector<std::string> args);
    };
}
