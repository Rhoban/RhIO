#pragma once

#include <map>
#include <list>
#include <string>
#include <ClientReq.hpp>
#include "Command.h"
#include "Terminal.h"

namespace RhIO
{
    class Shell
    {
        public:
            Shell(std::string server);

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

            /**
             * Get the remote client
             */
            ClientReq *getClient();

            /**
             * Current directory
             */
            void enterPath(std::string path);
            void upPath();
            void rootPath();
            std::string getPath();

        protected:
            ClientReq *client;
            bool terminate;
            std::map<std::string, Command*> commands;
            std::list<std::string> path;
            std::string server;
    };
}
