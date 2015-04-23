#include <stdio.h>
#include <ctype.h>
#include <list>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <RhIO.hpp>
#include "Stream.h"
#include "Shell.h"
#include "utils.h"

#include "Completion.h"

#include <commands/RemoteCommand.h>



#include <stdlib.h>
#include <fcntl.h>

namespace RhIO
{
    Shell::Shell(std::string server_)
        : server(server_), client(NULL), clientSub(NULL), stream(NULL), tree(NULL)
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
        std::cout << hostname;
        Terminal::clear();
        std::cout << ":";
        Terminal::setColor("blue", true);
        std::cout << getPath();
        Terminal::clear();
        std::cout << "# " << std::flush;
    }

    void Shell::sync()
    {
        std::cout << "Synchronizing..." << std::endl;

        // Downloading tree
        if (tree != NULL) {
            delete tree;
        }
        tree = new Node(client, "");

        // Updating the hostname
        if (auto value = getValue("/server/hostname")) {
            hostname = Node::toString(value);
        } else {
            hostname = "RhIO";
        }

        // Updating the commands
        std::vector<std::string> toDelete;
        for (auto entry : commands) {
            if (dynamic_cast<RemoteCommand*>(entry.second)) {
                delete entry.second;
                toDelete.push_back(entry.first);
            }
        }
        for (auto name : toDelete) {
            commands.erase(name);
        }
        updateCommands(tree);
    }

    void Shell::updateCommands(Node *node)
    {
        for (auto name : node->getCommands()) {
            registerCommand(new RemoteCommand(node->getPath(), name, client->getCommandDesc(name)));
        }
        for (auto entry : node->children) {
            updateCommands(entry.second);
        }
    }

    void Shell::run()
    {

        terminal_set_ioconfig();

        std::string reqServer = "tcp://"+server+":"+ServerRepPort;
        std::string subServer = "tcp://"+server+":"+ServerPubPort;
        terminate = false;
        std::cout << "RhIO, connecting to " << server << std::endl;
        client = new ClientReq(reqServer);
        clientSub = new ClientSub(subServer);
        stream = new Stream(this);

        sync();

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
        std::string prev_cmd("");
        bool completion_mode=false;
        int completion_select=0;
        std::deque<std::string> completion_matches;
        std::vector<std::string> splitted_cmd;
        std::string cur_comp_line;
        std::string lastcmd;
        bool lastisspace=false;

        while(!done)
        {
            if ((c = getchar())>0)
            {
                switch(c)
                {
                    case 0x0a: //enter

                        putchar(c);
                        done=true;

                        prev_cmd="";
                        if(shell_history.size()>0)
                            prev_cmd=shell_history.back();

                        if(line.compare("")!=0 && line.compare(prev_cmd)!=0) //store in history if non null and different than the last cmd
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

                        if(cursorpos<line.size() )
                        {
                            // Terminal::cursorNLeft(cursorpos);
                            // Terminal::cursorNRight(line.size());
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


                    case 0x7f: //backspace
                        if(line.size()>0)
                        {


                            if(cursorpos>0)
                                line.erase(cursorpos-1,1);
                            Terminal::clearLine();
                            displayPrompt();


                            std::cout<<line;

                            Terminal::cursorNLeft(line.size()+1);

                            Terminal::cursorNRight(cursorpos);
                            if(cursorpos>0)
                                cursorpos--;
                        }
                        break;

                    case 0x12: //Ctrl-r history completion

                            //TODO
                        cur_comp_line=line;
                        line="";
                        for(std::deque<std::string>::iterator cmd_it=shell_history.begin(); cmd_it!=shell_history.end();++cmd_it)
                        {
                            if((*cmd_it).compare(0,cur_comp_line.size(),cur_comp_line)==0)
                                completion_matches.push_back(*cmd_it);
                        }



                        if(completion_matches.size()==1) //one solution, we are done
                        {

                            cur_comp_line=completion_matches[0];

                            line+=cur_comp_line;
                            Terminal::clearLine();
                            displayPrompt();
                            std::cout<<line;
                            cursorpos=line.size();
                            break;
                        }



                        std::cout<<std::endl;
                        for(std::deque<std::string>::iterator it=completion_matches.begin(); it!=completion_matches.end();++it)
                            std::cout<<*it<<'\t';
                        std::cout<<std::endl;

                            //lazy longest common substring (there is almost 2 elements)
                        cur_comp_line=Completion::getSubstring(completion_matches);


                        line+=cur_comp_line;


                        Terminal::clearLine();
                        displayPrompt();
                        std::cout<<line;
                        cursorpos=line.size();

                        break;

                    case 0x09: //TAB completion
                            //completion_mode
                            //completion_select
                        completion_matches.clear();
                        splitted_cmd.clear();
                        cur_comp_line="";
                        lastcmd="";
                        lastisspace=false;
                            //look at the line and split all the commands separated by a space
                            //work on the last one

                        if(line.size()>0)
                        {
                            if(line.back()==' '){

                                lastisspace=true;

                                line.pop_back();
                                lastcmd=line;
                            }
                            splitted_cmd=Completion::split(line,' ');

                            if(splitted_cmd.size()>1)
                            {

                                line="";
                                cur_comp_line=splitted_cmd.back();
                                splitted_cmd.pop_back();
                                for(std::vector<std::string>::iterator it=splitted_cmd.begin();it!=splitted_cmd.end();++it)
                                    line+=*it+" ";

                            }
                            else{
                                cur_comp_line=line;
                                line="";
                            }

                        }
                        else{
                            cur_comp_line="";
                        }
                        if(lastisspace){
                            cur_comp_line="";
                            line=lastcmd+' ';

                        }





                            // simple completion on commands

                            // look for matching on commands
                        for(std::map<std::string, Command*>::iterator cmd_it=commands.begin(); cmd_it!=commands.end();++cmd_it)
                        {
                            if(cmd_it->first.compare(0,cur_comp_line.size(),cur_comp_line)==0)
                                completion_matches.push_back(cmd_it->first);
                        }



                        if(completion_matches.size()==1) //one solution, we are done
                        {

                            cur_comp_line=completion_matches[0];

                            line+=cur_comp_line;
                            Terminal::clearLine();
                            displayPrompt();
                            std::cout<<line;
                            cursorpos=line.size();
                            break;
                        }



                        std::cout<<std::endl;
                        for(std::deque<std::string>::iterator it=completion_matches.begin(); it!=completion_matches.end();++it)
                            std::cout<<*it<<'\t';
                        std::cout<<std::endl;

                            //lazy longest common substring (there is almost 2 elements)
                        cur_comp_line=Completion::getSubstring(completion_matches);


                        line+=cur_comp_line;


                        Terminal::clearLine();
                        displayPrompt();
                        std::cout<<line;
                        cursorpos=line.size();

                        break;


                            // if not esc_mode -> fall to default
                    case 0x41: //up
                        if(esc_mode && c==0x41)
                        {

                            if(shell_history.size()>0 && hist_it!= shell_history.begin())
                            {
                                line=*--hist_it;
                                cursorpos=line.size();
                                Terminal::clearLine();
                                displayPrompt();
                                std::cout<<line;
                            }
                            esc_mode=false;
                            break;
                        }


                    case 0x42: //down
                        if(esc_mode && c==0x42)
                        {
                            if(shell_history.size()>0 && hist_it!= shell_history.end())
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
                            break;
                        }
                        // break;

                    case 0x43: //right
                        if(esc_mode && c==0x43)
                        {

                            if(cursorpos<line.size())
                            {
                                Terminal::cursorRight();
                                cursorpos++;
                            }
                            esc_mode=false;
                            break;
                        }
                        // break;

                    case 0x44: //left
                        if(esc_mode && c==0x44)
                        {

                            if(cursorpos>0)
                            {
                                Terminal::cursorLeft();
                                cursorpos--;
                            }
                            esc_mode=false;
                            break;
                        }
                        // break;


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
        line="";
        return line;
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
                try {
                    commands[command]->process(argsV);
                } catch (std::string err) {
                    Terminal::setColor("red", true);
                    std::cout << "Error: " << err << std::endl;
                    Terminal::clear();
                }
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
        if (commands.count(command->getName())) {
            delete commands[command->getName()];
        }
        commands[command->getName()] = command;
    }

    std::map<std::string, Command*> Shell::getCommands()
    {
        return commands;
    }

    Command *Shell::getCommand(std::string command)
    {
        if (commands.count(command)) {
            return commands[command];
        }

        return NULL;
    }

    ClientReq *Shell::getClient()
    {
        return client;
    }

    ClientSub *Shell::getClientSub()
    {
        return clientSub;
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

    bool Shell::goToPath(std::string spath)
    {
        if (auto node = getNode(spath)) {
            auto parts = pathToParts(node->getPath());

            path.clear();
            for (auto part : parts) {
                path.push_back(part);
            }
            return true;
        } else {
            return false;
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

    Stream *Shell::getStream()
    {
        return stream;
    }

    ValueBase *Shell::getValue(std::string path)
    {
        return getNodeValue(path).value;
    }

    Node *Shell::getCurrentNode()
    {
        return getNode();
    }

    NodePool Shell::poolForNode(Node *node)
    {
        NodePool pool;
        for (auto nodeVal : node->getAll()) {
            Node::get(this, nodeVal);
            pool.push_back(nodeVal);
        }

        return pool;
    }

    NodePool Shell::getPool(std::vector<std::string> names, int start)
    {
        if (names.size()-start <= 0) {
            return poolForNode(getCurrentNode());
        } else {
            NodePool pool;
            for (int k=start; k<names.size(); k++) {
                auto val = getNodeValue(names[k]);
                if (val.value == NULL) {
                    std::ostringstream oss;
                    oss << "Unknown parameter: " << names[k];
                    throw oss.str();
                }
                Node::get(this, val);
                pool.push_back(val);
            }

            return pool;
        }
    }

    void Shell::streamWait(NodePool *pool)
    {
        stream->addPool(pool);
        std::string line;
        std::getline(std::cin, line);
        stream->removePool(pool);
    }
}
