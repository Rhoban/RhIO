#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace RhIO
{
    class Node;
    class Shell;
    class Command
    {
        public:
            virtual ~Command();
            virtual std::string getName()=0;
            virtual std::string getDesc()=0;
            virtual std::string getUsage();
            virtual void process(std::vector<std::string> args)=0;

            void setShell(Shell *shell_);
            void errorUsage();

            /**
             * Get the node corresponding to given args
             */
            Node *getNode(std::vector<std::string> args);

            /**
             * Getting the stream where data should be put
             */
            std::ostream *getStream(std::vector<std::string> &args);
            void clearStream();

        protected:
            std::ofstream ofs;
            Shell *shell;
    };
}
