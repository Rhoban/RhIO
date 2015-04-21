#pragma once

#include "Command.h"

namespace Rhio
{
    class TuneCommand : public Command
    {
        public:
            virtual std::string getName();
            virtual std::string getDesc();
            virtual std::string getUsage();
            virtual void process(std::list<std::string> args);
    };
}
