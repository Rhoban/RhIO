#include <stdio.h>
#include <ctype.h>
#include <list>
#include <iostream>
#include <string>
#include <algorithm> 
#include <functional> 
#include "Shell.h"
#include "utils.h"

namespace RhIO
{
    Shell::Shell(std::string server_)
        : server(server_), client(NULL)
    {
    }

    void Shell::run()
    {
        terminate = false;
        Terminal::setColor("white", true);
        std::cout << "Rhoban I/O shell, welcome!" << std::endl;
        std::cout << "Connecting to " << server << std::endl;
        client = new ClientReq(server);
        std::cout << "Downloading the tree..." << std::endl;
        tree = new Values(client, "");
        Terminal::clear();

        // Reading lines from stdin
        while (!terminate && !feof(stdin)) {
            Terminal::setColor("yellow", true);
            std::cout << "RhIO";
            Terminal::clear();
            std::cout << ":/";
            Terminal::setColor("blue", true);
            std::cout << getPath();
            Terminal::clear();
            std::cout << "# " << std::flush;
            std::string line;
            std::getline(std::cin, line);
            parse(line);
        }
        if (feof(stdin)) {
            std::cout << std::endl << std::flush;
        }
    }

    void Shell::parse(std::string line)
    {
        // Try to interpret command as a set
        for (int i=0; i<line.size(); i++) {
            if (line[i] == '=') {
                std::string lvalue = line.substr(0, i);
                std::string rvalue = line.substr(i+1);
                trim(lvalue);
                trim(rvalue);
                set(lvalue, rvalue);
                return;
            }
        }

        // Try to split line into parts and execute it
        std::list<std::string> parts;
        std::string part;

        for (int i=0; i<line.size(); i++) {
            if (std::isspace(line[i])) {
                if (part != "") {
                    parts.push_back(part);
                    part = "";
                }
            } else {
                part += line[i];
            }
        }
        if (part != "") {
            parts.push_back(part);
        }

        if (parts.size()) {
            auto command = parts.front();
            parts.pop_front();
            process(command, parts);
        }
    }

    void Shell::process(std::string command, std::list<std::string> args)
    {
        // First, try to quit/exit
        if (command == "quit" || command == "exit") {
            terminate = true;
        } else {
            // Checking for the command in the list
            if (commands.count(command)) {
                std::vector<std::string> argsV;
                for (auto part : args) {
                    argsV.push_back(part);
                }
                commands[command]->process(argsV);
            } else {
                Terminal::setColor("red", true);
                std::cout << "Unknown command: " << command << std::endl;
                Terminal::clear();
            }
        }
    }

    void Shell::set(std::string lvalue, std::string rvalue)
    {
        Terminal::setColor("yellow", true);
        std::cout << "[Not implemented] Setting " << lvalue << " to " << rvalue << std::endl;
        Terminal::clear();
    }

    void Shell::registerCommand(Command *command)
    {
        command->setShell(this);
        commands[command->getName()] = command;
    }

    std::map<std::string, Command*> Shell::getCommands()
    {
        return commands;
    }

    ClientReq *Shell::getClient()
    {
        return client;
    }
            
    void Shell::enterPath(std::string path_)
    {
        path.push_back(path_);
    }

    void Shell::upPath()
    {
        if (path.size()) {
            path.pop_back();
        }
    }

    void Shell::goToPath(std::string spath)
    {
        if (getNode(spath)) {
            auto parts = pathToParts(spath);

            path.clear();
            for (auto part : parts) {
                path.push_back(part);
            }
        }
    }

    std::string Shell::getPath()
    {
        std::string p = "";

        for (auto part : path) {
            if (p != "") {
                p += "/";
            }
            p += part;
        }

        if (p == "") {
            p = "";
        }

        return p;
    }

    std::vector<std::string> Shell::pathToParts(std::string spath)
    {
        auto parts = split(spath, '/');
        std::vector<std::string> path;

        for (auto part : parts) {
            if (part != "") {
                path.push_back(part);
            }
        }
        
        return path;
    }

    Values *Shell::getNode(std::string spath)
    {
        auto path = pathToParts(spath);

        Values *node = tree;
        for (auto part : path) {
            node = node->getChild(part);
            if (node == NULL) {
                return NULL;
            }
        }
        return node;
    }
            
    Values *Shell::getCurrentNode()
    {
        return getNode(getPath());
    }
}
