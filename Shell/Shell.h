#pragma once

#include <map>
#include <list>
#include <string>
#include "Command.h"
#include "Terminal.h"

namespace Rhio
{
    class Shell
    {
        public:
            Shell();

            /**
             * Runs the interactive shell, will get lines from stdin
             */
            void run();

            /**
             * Parse a command line typed by the user
             */
            void parse(std::string line);

            /**
             * Process a command typed by the user
             */
            void process(std::string command, std::list<std::string> args);

            /**
             * Sets a value
             */
            void set(std::string lvalue, std::string rvalue);

            /**
             * Register a command to the shell
             */
            void registerCommand(Command *command);
            
            /**
             * Get the list of commands
             */
            std::map<std::string, Command*> getCommands();

        protected:
            bool terminate;
            std::map<std::string, Command*> commands;
    };
}
