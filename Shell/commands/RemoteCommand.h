#pragma once

#include "Command.h"

namespace RhIO
{
    class RemoteCommand : public Command
    {
        public:
            RemoteCommand(std::string origin, std::string name, std::string desc);

            virtual std::string getName();
            virtual std::string getDesc();
            virtual std::string getOrigin();
            virtual void process(std::vector<std::string> args);

        protected:
            std::string origin;
            std::string name;
            std::string desc;
    };
}
