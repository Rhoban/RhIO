#pragma once

#include <map>
#include <list>
#include <string>
#include <RhIOClient.hpp>
#include "commands/Command.h"
#include "Terminal.h"
#include "Node.h"
#include "NodePool.h"
#include <termios.h>
#include <deque>
#include <fstream>

#define MAX_HISTORY 1000

namespace RhIO
{
    class StreamManager;
    class Shell
    {
        public:
            Shell(std::string server);
            ~Shell();

            /**
             * Updates the local node tree
             */
            void sync(bool display=true);
            void updateCommands();

            /**
             * Runs the interactive shell, will get lines from stdin
             */
            void run(std::string cmd="");
            bool quit();
            void terminal_set_ioconfig();
            void terminal_set_config();

            /**
             * Read the history file in HOME/.rhio_history
             */
            void readHistory();

            /**
             * Append a line in history file in HOME/.rhio_history
             */
            void writeHistory(std::string line);

            /**
             * Display the prompt...
             */
            void displayPrompt();

            /**
             * Read the command line
             */
            std::string getLine();

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
             * Commands handling
             */
            void registerCommand(Command *command);
            std::map<std::string, Command*> getCommands();
            Command *getCommand(std::string command);

            /**
             * Get the remote client
             */
            ClientReq *getClient();
            ClientSub *getClientSub();

            /**
             * Get Node for a given path
             */
            Node *getNode(std::string path="");
            Node *getCurrentNode();
            ValueBase *getValue(std::string path);
            NodeValue getNodeValue(std::string path);
            NodeStream getNodeStream(std::string path);
            NodeFrame getNodeFrame(std::string path);

            /**
             * Current directory
             */
            void enterPath(std::string path);
            void upPath();
            std::vector<std::string> pathToParts(std::string spath);
            bool goToPath(std::string path);
            std::string getPath();

            /**
             * Add the poll to the stream, and wait the user to press enter, then
             * remove the pool from the stream
             */
            void streamWait(NodePool *pool, Command *command);
            void wait(Command *command);
            StreamManager *getStream();
            NodePool poolForNode(Node *node);
            NodePool getPool(std::vector<std::string> names, int start=0);

            Node *tree;
            struct termios termsave;
            struct termios termshell;
            std::deque<std::string> shell_history;
            std::fstream history_file;
            std::string history_path;
            /**
             * Get all the possibilities at a certain point
             */
            std::vector<std::string> getPossibilities(std::string prefix);

            /**
             * Adds an alias
             */
            void addAlias(std::string from, std::string to);

            void getFromServer(NodeValue value);
            void setToServer(NodeValue value);
            void setFromString(NodeValue value, std::string str);
            void setFromNumber(NodeValue value, float number);

            bool hasInput();

        protected:
            std::vector<char> chars;
            Command *currentCommand;
            std::map<std::string, std::string> aliases;
            std::string hostname;
            ClientReq *client;
            ClientSub *clientSub;
            StreamManager *stream;
            bool terminate;
            std::map<std::string, Command*> commands;
            std::list<std::string> path;
            std::string server;

    };
}
