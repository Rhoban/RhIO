#pragma once

#include "Command.h"

namespace Rhio
{
    class HelpCommand : public Command
    {
        public:
            virtual std::string getName();
            virtual std::string getDesc();
            virtual void process(std::list<std::string> args);
    };
}
