#include <stdio.h>
#include <ctype.h>
#include <list>
#include <iostream>
#include <string>
#include <algorithm> 
#include <functional> 
#include "Shell.h"
#include "utils.h"

namespace Rhio
{
    Shell::Shell()
    {
    }

    void Shell::run()
    {
        terminate = false;
        Terminal::setColor("white", true);
        std::cout << "Rhoban I/O shell, welcome!" << std::endl;
        Terminal::clear();

        // Reading lines from stdin
        while (!terminate && !feof(stdin)) {
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
                commands[command]->process(args);
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
}
