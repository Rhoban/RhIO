#include <stdio.h>
#include <ctype.h>
#include <list>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <RhIO.hpp>
#include "Shell.h"
#include "utils.h"


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

namespace RhIO
{
    Shell::Shell(std::string server_)
        : server(server_), client(NULL), clientSub(NULL)
    {
    }

    void Shell::terminal_set_ioconfig() {
        struct termios custom;
        int fd=fileno(stdin);
        tcgetattr(fd, &termsave);
        custom=termsave;
        custom.c_lflag &= ~(ICANON|ECHO);
        tcsetattr(fd,TCSANOW,&custom);
        // fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0)|O_NONBLOCK);
        fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0)); //blocking
    }

    void Shell::displayPrompt()
    {
        Terminal::setColor("yellow", true);
        std::cout << "RhIO";
        Terminal::clear();
        std::cout << ":";
        Terminal::setColor("blue", true);
        std::cout << getPath();
        Terminal::clear();
        std::cout << "# " << std::flush;
    }

    void Shell::run()
    {

        terminal_set_ioconfig();

        std::string reqServer = "tcp://"+server+":"+ServerRepPort;
        std::string subServer = "tcp://"+server+":"+ServerPubPort;
        terminate = false;
        Terminal::setColor("white", true);
        std::cout << "Rhoban I/O shell, welcome!" << std::endl;
        std::cout << "Connecting to " << server << std::endl;
        client = new ClientReq(reqServer);
        clientSub = new ClientSub(subServer);
        std::cout << "Downloading the tree..." << std::endl;
        tree = new Node(client, "");
        Terminal::clear();

        // Reading lines from stdin
        while (!terminate ) {
            displayPrompt();
            std::string line;
            // std::getline(std::cin, line);
            line=getLine();
            parse(line);
        }

        tcsetattr(fileno(stdin),TCSANOW,&termsave);
        std::cout << std::endl << std::flush;

    }

    std::string Shell::getLine()
    {

        char c;
        std::string line("");
        bool done=false;
        bool esc_mode=false;
        std::deque<std::string>::iterator hist_it=shell_history.end();
        int cursorpos=0;
        std::string lastcmd("");

        while(!done)
        {
            if ((c = getchar())>0)
            {
                switch(c)
                {
                    case 0x0a: //enter

                        putchar(c);
                        done=true;

                        lastcmd="";
                        if(shell_history.size()>0)
                            lastcmd=shell_history.back();

                        if(line.compare("")!=0 and line.compare(lastcmd)!=0) //store in history if non null and different than the last cmd
                        {
                            shell_history.push_back(line);
                            if(shell_history.size()>MAX_HISTORY)
                                shell_history.pop_front();
                            hist_it=shell_history.begin();
                        }
                        return line;
                        break;//useless

                    case 0x01: //Ctrl-a goto begin of line
                        Terminal::clearLine();
                        displayPrompt();
                        std::cout<<line;
                        cursorpos=0;
                        if(line.size()>0)
                            Terminal::cursorNLeft(line.size());

                        break;

                    case 0x05: //Ctrl-e goto end of line
                        Terminal::clearLine();
                        displayPrompt();
                        std::cout<<line;

                        if(cursorpos<line.size())
                        {
                            Terminal::cursorNRight(line.size()-cursorpos);
                            cursorpos=line.size();
                        }

                        break;

                    case 0xc: //Ctrl-l clear screen
                        Terminal::clearScreen();
                        Terminal::clearLine();
                        Terminal::initCursor();
                        displayPrompt();
                        line="";
                        cursorpos=0;
                        break;

                    case 0x1b: //begin break mode (arrows)
                        esc_mode=true;

                        break;

                    case 0x5b: //just after 0x1b

                        break;

                    case 0x41: //up
                        if(esc_mode)
                        {

                            if(shell_history.size()>0 and hist_it!= shell_history.begin())
                            {
                                line=*--hist_it;
                                cursorpos=line.size();
                                Terminal::clearLine();
                                displayPrompt();
                                std::cout<<line;
                            }
                            esc_mode=false;
                        }
                        break;

                    case 0x42: //down
                        if(esc_mode)
                        {
                            if(shell_history.size()>0 and hist_it!= shell_history.end())
                            {
                                line=*hist_it++;
                                cursorpos=line.size();
                                Terminal::clearLine();
                                displayPrompt();
                                std::cout<<line;
                            }
                            else if( hist_it== shell_history.end())
                            {
                                Terminal::clearLine();
                                displayPrompt();
                                line="";
                                cursorpos=0;
                            }

                            esc_mode=false;
                        }
                        break;

                    case 0x43: //right
                        if(esc_mode)
                        {

                            if(cursorpos<line.size())
                            {
                                Terminal::cursorRight();
                                cursorpos++;
                            }
                            esc_mode=false;
                        }
                        break;

                    case 0x44: //left
                        if(esc_mode)
                        {

                            if(cursorpos>0)
                            {
                                Terminal::cursorLeft();
                                cursorpos--;
                            }
                            esc_mode=false;
                        }
                        break;

                    case 0x7f: //backspace
                        if(line.size()>0)
                        {
                            line.pop_back();
                            Terminal::clearLine();
                            displayPrompt();
                            cursorpos--;
                            std::cout<<line;
                        }
                        break;

                    default:

                        if(line.size()>0)
                        {
                            std::string tmp("");
                            tmp+=c;
                            line.insert(cursorpos,tmp);
                        }
                        else{
                            line+=c;
                        }
                        cursorpos++;
                        Terminal::clearLine();
                        displayPrompt();

                        std::cout<<line;

                        if(line.size()-cursorpos>0)
                            Terminal::cursorNLeft(line.size()-cursorpos);

                        break;
                }
            }
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
                auto nodeValue = getNodeValue(command);
                auto value = nodeValue.value;

                if (value) {
                    Node::get(this, nodeValue);
                    std::cout << command << "=" << Node::toString(value) << std::endl;
                } else {
                    Terminal::setColor("red", true);
                    std::cout << "Unknown command: " << command << std::endl;
                    Terminal::clear();
                }
            }
        }
    }

    void Shell::set(std::string lvalue, std::string rvalue)
    {
        auto node = getCurrentNode();
        auto nodeValue = getNodeValue(lvalue);
        auto value = nodeValue.value;

        if (value) {
            Node::setFromString(this, nodeValue, rvalue);
        } else {
            Terminal::setColor("red", true);
            std::cout << "Unknown parameter: " << lvalue << std::endl;
            Terminal::clear();
        }
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
        if (auto node = getNode(spath)) {
            auto parts = pathToParts(node->getPath());

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

    Node *Shell::getNode(std::string spath)
    {
        if (spath.size()==0 || spath[0]!='/') {
            auto myPath = getPath();
            if (myPath != "") {
                myPath += "/";
            }
            spath = myPath + spath;
        }

        auto path = pathToParts(spath);

        Node *node = tree;
        for (auto part : path) {
            node = node->getChild(part);
            if (node == NULL) {
                return NULL;
            }
        }
        return node;
    }

    NodeValue Shell::getNodeValue(std::string path)
    {
        auto parts = pathToParts(path);

        if (parts.size() == 0) {
            return NodeValue(NULL, NULL);
        }

        // Child name
        auto name = parts[parts.size()-1];
        parts.pop_back();

        // Creating value path
        std::string prefix = "";
        for (auto part : parts) {
            if (prefix != "") prefix += "/";
            prefix += part;
        }
        if (path[0] == '/') {
            prefix = "/" + prefix;
        }

        // Getting node
        auto node = getNode(prefix);
        if (node == NULL) {
            return NodeValue(NULL, NULL);
        }

        return node->getNodeValue(name);
    }

    ValueBase *Shell::getValue(std::string path)
    {
        return getNodeValue(path).value;
    }

    Node *Shell::getCurrentNode()
    {
        return getNode();
    }
}
