#pragma once

#include <string>
#include <vector>

namespace RhIO
{
    class Shell;
    class Command
    {
        public:
            virtual std::string getName()=0;
            virtual std::string getDesc()=0;
            virtual std::string getUsage();
            virtual void process(std::vector<std::string> args)=0;

            void setShell(Shell *shell_);
            void errorUsage();

        protected:
            Shell *shell;
    };
}
