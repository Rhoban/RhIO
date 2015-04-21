#pragma once

#include <string>
#include <list>

namespace Rhio
{
    class Shell;
    class Command
    {
        public:
            virtual std::string getName()=0;
            virtual std::string getDesc()=0;
            virtual std::string getUsage();
            virtual void process(std::list<std::string> args)=0;

            void setShell(Shell *shell_);

        protected:
            Shell *shell;
    };
}
