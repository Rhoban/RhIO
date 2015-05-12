#include <stdexcept>
#include <stdio.h>
#include <ctype.h>
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <RhIO.hpp>
#include "StreamManager.h"
#include "Shell.h"
#include "utils.h"
#include <set>
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

    Shell::~Shell()
    {
        if (history_file.is_open())
            history_file.close();
    }

    void Shell::terminal_set_ioconfig() {
        struct termios custom;
        int fd = fileno(stdin);
        tcgetattr(fd, &termsave);
        termshell = termsave;
        termshell.c_lflag &= ~(ICANON|ECHO);
        tcsetattr(fd, TCSANOW, &termshell);
        // fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0)|O_NONBLOCK);
        fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0)); //blocking
    }

    void Shell::terminal_set_config()
    {
        int fd = fileno(stdin);
        tcsetattr(fd, TCSANOW, &termshell);
    }

    void Shell::readHistory()
    {

        // std::cout<<"HOME: "<<hist_path<<std::endl;
        history_file.open(history_path,std::fstream::in);
        std::string line;
        if (history_file.is_open())
        {
            while ( getline (history_file,line) )
            {
                shell_history.push_back(line);

            }
        }
        history_file.close();
    }

    void Shell::writeHistory(std::string line)
    {
        if(history_file.is_open())
        {
            history_file<<line<<std::endl;
            history_file.flush();
        }

    }

    void Shell::displayPrompt()
    {
        Terminal::setColor("yellow", true);
        std::cout << hostname;
        Terminal::clear();
        std::cout << ":";
        Terminal::setColor("blue", true);
        std::cout << "/" << getPath();
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

        updateCommands();
    }

    void Shell::updateCommands()
    {
        // Cleaning current remote commands
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

        // Getting commands
        auto commands = client->listAllCommands();
        for (auto name : commands) {
            registerCommand(new RemoteCommand(name, client->commandDescription(name)));
        }
    }

    void Shell::run()
    {

        const char *homedir;
        if ((homedir = getenv("HOME")) != NULL)
        {
            history_path=homedir;
            history_path+="/.rhio_history";
        }
        readHistory();
        // std::cout<<"PATH: "<<history_path<<std::endl;
        history_file.open(history_path, std::fstream::out | std::fstream::app);

        terminal_set_ioconfig();

        std::string reqServer = "tcp://"+server+":"+ServerRepPort;
        std::string subServer = "tcp://"+server+":"+ServerPubPort;
        terminate = false;
        std::cout << "RhIO, connecting to " << server << std::endl;
        client = new ClientReq(reqServer);
        clientSub = new ClientSub(subServer);
        stream = new StreamManager(this);

        sync();

        // Reading lines from stdin
        while (!terminate ) {
            displayPrompt();
            std::string line;
            // std::getline(std::cin, line);
            line = getLine();
            parse(line);
            terminal_set_config();
        }

        quit();
    }

    void Shell::quit()
    {
        tcsetattr(fileno(stdin),TCSANOW,&termsave);
        std::cout << std::endl << std::flush;
    }

    std::string Shell::getLine()
    {

        char c;

        std::string line("");
        std::string prev_cmd("");
        std::string completion_selected("");
        std::string lineback;
        std::string cmd_list("");
        std::string cur_comp_line("");
        std::string lastcmd("");
        std::string current_path("");

        bool done=false;
        bool esc_mode=false;
        bool completion_mode=false;
        bool history_mode=false;
        bool lastisspace=false;


        std::deque<std::string>::iterator hist_it=shell_history.end();

        std::deque<std::string> completion_matches;

        std::vector<std::string> splitted_cmd;
        std::vector<std::string> paths;
        std::vector<std::string> paths_to_print;
        std::vector<std::string> cmd_to_print;
        int print_len=0;
        int cursorpos=0;
        int completion_select=0;

        while(!done)
        {
            if ((c = getchar())>0)
            {
                switch(c)
                {
                    case 0x0a: //enter

                        if(completion_mode || history_mode) //we have selected a choice in the completion
                        {
                            line=completion_selected;
                        }
                        // else
                        //     putchar(c); ??
                        putchar(c);
                        done=true;

                        prev_cmd="";
                        if(shell_history.size()>0)
                            prev_cmd=shell_history.back();

                        if(line.compare("")!=0 && line.compare(prev_cmd)!=0) //store in history if non null and different than the last cmd
                        {
                            shell_history.push_back(line);
                            if(line.compare("quit")!=0 && line.compare("exit")!=0)
                                writeHistory(line);

                            if(shell_history.size()>MAX_HISTORY)
                                shell_history.pop_front();
                            hist_it=shell_history.begin();

                        }
                        completion_mode=false;
                        history_mode=false;

                        return line;
                        break;//useless

                    case 0x01: //Ctrl-a goto begin of line
                        completion_mode=false;
                        history_mode=false;
                        Terminal::clearLine();
                        displayPrompt();
                        std::cout<<line;
                        cursorpos=0;
                        if(line.size()>0)
                            Terminal::cursorNLeft(line.size());

                        break;

                    case 0x05: //Ctrl-e goto end of line
                        completion_mode=false;
                        history_mode=false;
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
                        completion_mode=false;
                        history_mode=false;
                        Terminal::clearScreen();
                        Terminal::clearLine();
                        Terminal::initCursor();
                        displayPrompt();
                        line="";
                        cursorpos=0;
                        break;

                    case 0x1b: //begin break mode (arrows)
                        completion_mode=false;
                        history_mode=false;
                        esc_mode=true;
                        break;

                    case 0x7e: //after 33 for suppr
                        completion_mode=false;
                        history_mode=false;
                        if(esc_mode)
                        {
                            esc_mode=false;

                            if(line.size()>0)
                            {
                                if(cursorpos>=0)
                                    line.erase(cursorpos,1);
                                Terminal::clearLine();
                                displayPrompt();

                                std::cout<<line;

                                Terminal::cursorNLeft(line.size());
                                if(cursorpos>0)
                                    Terminal::cursorNRight(cursorpos);
                            }

                        }
                        break;

                    case 0x7f: //backspace
                        completion_mode=false;
                        history_mode=false;
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

                        completion_mode=false;


                        if(history_mode) //same as completion_mode navigation
                        {

                            completion_selected="";
                            print_len=0;
                            int i=0;
                            std::cout<<std::endl;
                            Terminal::setColor("white", true);
                            for(std::vector<std::string>::iterator it=cmd_to_print.begin(); it!=cmd_to_print.end();++it)
                            {
                                print_len+=(*it).size();
                                if(print_len>20){
                                    std::cout<<std::endl;
                                    print_len=0;
                                }
                                if(completion_select<cmd_to_print.size() && completion_select==i )
                                {
                                    Terminal::setBColor("blue", true);
                                    std::cout<<*it;
                                    Terminal::clear();
                                    std::cout<<"\t\t";
                                    Terminal::setColor("white", true);
                                    completion_selected=*it;
                                }
                                else
                                    std::cout<<*it<<"\t\t";
                                i++;
                            }

                            Terminal::clear();
                            std::cout<<std::endl;

                            completion_select++;
                            if(completion_select>cmd_to_print.size())
                                completion_select=0;
                            break;
                        }



                        history_mode=true;
                            //End history mode
                        cmd_to_print.clear();
                        completion_matches.clear();
                        cur_comp_line=line;
                        line="";
                        for(std::deque<std::string>::iterator hist_it=shell_history.begin(); hist_it!=shell_history.end();++hist_it)
                        {
                            if((*hist_it).compare(0,cur_comp_line.size(),cur_comp_line)==0)
                            {
                                    //bof

                                //     //check if it is a cmd
                                // for(std::map<std::string, Command*>::iterator cmd_it=commands.begin(); cmd_it!=commands.end();++cmd_it)
                                // {
                                //     if((*hist_it).compare(0,cmd_it->first.size(),cmd_it->first)==0)
                                //     {
                                //         std::cout<<"DEBUG hist "<<*hist_it<<" "<<cmd_it->first<<std::endl;
                                //         cmd_to_print.push_back(*hist_it);
                                //     }
                                // }
                                    ////
                                completion_matches.push_back(*hist_it);
                                cmd_to_print.push_back(*hist_it);
                            }
                        }

                            //filter out duplicates
                        unique_vect(completion_matches);
                        unique_vect(cmd_to_print); //will be used for the history_mode

                        if(completion_matches.size()==1) //if there is only one solution, we are done
                        {

                            cur_comp_line=completion_matches[0];

                            line+=cur_comp_line;
                            Terminal::clearLine();
                            displayPrompt();
                            std::cout<<line;
                            cursorpos=line.size();
                            break;
                        }


                        print_len=0;
                        std::cout<<std::endl;
                        Terminal::setColor("white", true);
                        for(std::deque<std::string>::iterator it=completion_matches.begin(); it!=completion_matches.end();++it)
                        {
                            print_len+=(*it).size();
                            if(print_len>20){
                                std::cout<<std::endl;
                                print_len=0;
                            }



                            std::cout<<*it<<"\t\t";

                        }
                        Terminal::clear();
                        std::cout<<std::endl;

                            //longest common substring (there is almost 2 elements)
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

                            //TODO cleanup, it becomes quite messy....
                        history_mode=false;
                        // completion_mode=false; //desactivate that stuff

                        if(completion_mode) //we keep pressing tab so let's navigate through solutions
                        {
                            completion_selected="";
                            print_len=0;
                            int i=0;
                            std::cout<<std::endl;
                            Terminal::setColor("green", true);
                                // Terminal::setBColor("green", true);
                            for(std::vector<std::string>::iterator it=cmd_to_print.begin(); it!=cmd_to_print.end();++it)
                            {
                                print_len+=(*it).size();
                                if(print_len>20){
                                    std::cout<<std::endl;
                                    print_len=0;
                                }
                                if(completion_select<cmd_to_print.size() && completion_select==i )
                                {
                                    Terminal::setBColor("blue", true);
                                    std::cout<<*it;
                                    Terminal::clear();
                                    std::cout<<"\t\t";
                                    Terminal::setColor("green", true);
                                    completion_selected=cmd_list+*it;
                                }
                                else
                                    std::cout<<*it<<"\t\t";
                                i++;
                            }
                            i=0;
                            Terminal::setColor("blue", true);
                            for(std::vector<std::string>::iterator it=paths_to_print.begin(); it!=paths_to_print.end();++it)
                            {
                                print_len+=(*it).size();
                                if(print_len>20){
                                    std::cout<<std::endl;
                                    print_len=0;
                                }

                                if(completion_select>=cmd_to_print.size() && completion_select-cmd_to_print.size()==i)
                                {
                                    Terminal::setBColor("blue", true);
                                    std::cout<<*it;
                                    Terminal::clear();
                                    std::cout<<"\t\t";
                                    Terminal::setColor("blue", true);
                                    completion_selected=cmd_list+*it;
                                }
                                else
                                    std::cout<<*it<<"\t\t";
                                i++;


                            }
                            Terminal::clear();
                            std::cout<<std::endl;

                            completion_select++;
                            if(completion_select>cmd_to_print.size()+paths_to_print.size())
                                completion_select=0;
                            break;
                        }

                            //Else, normal completion, first tab


                        completion_matches.clear();
                        splitted_cmd.clear();
                        paths_to_print.clear();
                        cmd_to_print.clear();
                        cur_comp_line="";
                        lastcmd="";
                        lastisspace=false;

                        print_len=0;
                        lineback=line;
                        cmd_list="";
                        current_path="";
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
                                {
                                    line+=*it+" ";
                                    cmd_list+=*it+" ";
                                }

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
                            {
                                completion_matches.push_back(cmd_it->first);
                                cmd_to_print.push_back(cmd_it->first);
                            }
                        }


                        //     //just remove the last '/'
                        // if(cur_comp_line.back()=='/')
                        //     cur_comp_line.pop_back();

                            //also look for path
                        current_path=cur_comp_line;

                        // paths=getPossibilities();
                        paths=getPossibilities(cur_comp_line);
                        if(paths.size()==0)
                        {
                                //cut at last complete '/'
                            while(current_path.size()>0 && current_path.back()!='/')
                                current_path.pop_back();

                            paths=getPossibilities(current_path);
                        }

                        for(std::vector<std::string>::iterator p_it=paths.begin(); p_it!=paths.end();++p_it)
                        {
                            if((*p_it).compare(0,cur_comp_line.size(),cur_comp_line)==0)
                            {
                                    //only keep the current hierarchy level (cut at the next '/')
                                // if((*p_it).find("/",cur_comp_line.size())==std::string::npos)// || cur_comp_line.size()==0 || cur_comp_line.back()=='/')
                                {
                                    completion_matches.push_back(*p_it);
                                    paths_to_print.push_back(*p_it);
                                }
                            }
                        }

                            //filter out the duplicate
                        unique_vect(paths_to_print);

                        if(completion_matches.size()==1) //one solution, we are done
                        {


                            cur_comp_line=completion_matches[0];

                            line+=cur_comp_line;


                            // if(paths_to_print.size()==1) //so it is a path
                            // {
                            //     // line+='/';
                            //     // line+=current_path;
                            //     // line+='/';
                            //     //TODO lazy update path?
                            //     //remind, we have to handle the root / and . and ..
                            // }



                            Terminal::clearLine();
                            displayPrompt();
                            std::cout<<line;
                            cursorpos=line.size();
                            break;
                        }
                        else if(completion_matches.size()==0)
                        {
                            line+=cur_comp_line;
                        }
                        else
                            completion_mode=true;


                        // std::cout<<std::endl;
                        // for(std::deque<std::string>::iterator it=completion_matches.begin(); it!=completion_matches.end();++it)
                        //     std::cout<<*it<<'\t';
                        // std::cout<<std::endl;

                        std::cout<<std::endl;
                        Terminal::setColor("green", true);
                        // Terminal::setBColor("green", true);
                        for(std::vector<std::string>::iterator it=cmd_to_print.begin(); it!=cmd_to_print.end();++it)
                        {
                            print_len+=(*it).size();
                            if(print_len>20){
                                std::cout<<std::endl;
                                print_len=0;
                            }
                            std::cout<<*it<<"\t\t";
                        }
                        Terminal::setColor("blue", true);
                        for(std::vector<std::string>::iterator it=paths_to_print.begin(); it!=paths_to_print.end();++it)
                        {
                            print_len+=(*it).size();
                            if(print_len>20){
                                std::cout<<std::endl;
                                print_len=0;
                            }
                            std::cout<<*it<<"\t\t";
                        }
                        Terminal::clear();
                        std::cout<<std::endl;

                            //longest common substring (there is almost 2 elements)
                        cur_comp_line=Completion::getSubstring(completion_matches);


                        line+=cur_comp_line;
                        if(line.size()==0) //nothing found, we keep the line
                            line=lineback;

                        Terminal::clearLine();
                        displayPrompt();
                        std::cout<<line;
                        cursorpos=line.size();
                        completion_selected=line;
                        break;


                            // if not esc_mode -> fall to default
                    case 0x41: //up
                        completion_mode=false;
                        history_mode=false;
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
                        completion_mode=false;
                        history_mode=false;
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
                        completion_mode=false;
                        history_mode=false;
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
                        completion_mode=false;
                        history_mode=false;
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

                    case 0x5b: //just after 0x1b
                        completion_mode=false;
                        history_mode=false;
                        if(esc_mode && c==0x5b)
                            break;


                    case 0x33: //after 5b for suppr or char '3' and fall to default
                        completion_mode=false;
                        history_mode=false;
                        if(esc_mode && c==0x33)
                            break;


                    default:
                        completion_mode=false;
                        history_mode=false;
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

                        std::cout << line << std::flush;

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
                try {
                    set(lvalue, rvalue);
                } catch(...) {
                    Terminal::setColor("red", true);
                    std::cout << "Unable to set remote value (are you out of sync?)" << std::endl;
                    Terminal::clear();
                }
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
            // Applying alias
            if (aliases.count(command)) {
                command = aliases[command];
            }

            // Checking for the command in the list
            if (commands.count(command)) {
                std::vector<std::string> argsV;
                for (auto part : args) {
                    argsV.push_back(part);
                }
                try {
                    stream->setFrequency();
                    commands[command]->process(argsV);
                } catch (std::runtime_error error) {
                    Terminal::setColor("red", true);
                    std::cout << "Error: " << error.what() << std::endl;
                    Terminal::clear();
                }
            } else {
                auto nodeValue = getNodeValue(command);
                auto value = nodeValue.value;

                if (value) {
                    try {
                        getFromServer(nodeValue);
                        std::cout << command << "=" << Node::toString(value) << std::endl;
                    } catch(...) {
                        Terminal::setColor("red", true);
                        std::cout << "Unable to retrieve remote value (are you out of sync?)" << std::endl;
                        Terminal::clear();
                    }
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
            setFromString(nodeValue, rvalue);
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
            if (part == "..") {
                node = node->getParent();
            } else if (part != ".") {
                node = node->getChild(part);
            }
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

    NodeStream Shell::getNodeStream(std::string path)
    {
        auto parts = pathToParts(path);

        if (parts.size() != 0) {
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
            if (node != NULL) {
                // Getting stream
                for (auto stream : node->getStreams()) {
                    if (stream.name == name) {
                        return stream;
                    }
                }
            }
        }

        std::stringstream ss;
        ss << path << " is not a stream" << std::endl;
        throw std::runtime_error(ss.str());
    }

    StreamManager *Shell::getStream()
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
            getFromServer(nodeVal);
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
                    auto node = getNode(names[k]);
                    if (node == NULL) {
                        std::ostringstream oss;
                        oss << "Unknown parameter: " << names[k];
                        throw std::runtime_error(oss.str());
                    } else {
                        for (auto n : node->getAll()) {
                            getFromServer(n);
                            pool.push_back(n);
                        }
                    }
                } else {
                    getFromServer(val);
                    pool.push_back(val);
                }
            }

            return pool;
        }
    }

    void Shell::streamWait(NodePool *pool)
    {
        stream->addPool(this, pool);
        wait();
        stream->removePool(this, pool);
    }

    void Shell::wait()
    {
        std::string line;
        std::getline(std::cin, line);
    }

    std::vector<std::string> Shell::getPossibilities(std::string prefix)
    {
        std::vector<std::string> possibilities;

        if (auto node = getNode(prefix)) {
            // Adding children
            for (NodeValue nodeValue : node->getAll()) {
                auto name = prefix+nodeValue.value->name;
                possibilities.push_back(name);
            }

            // Adding streams
            for (auto stream : node->getStreams()) {
                auto name = prefix+stream.name;
                possibilities.push_back(name);
            }

            // Adding childs
            for (auto entry : node->getChildren()) {
                auto name = prefix+entry;
                possibilities.push_back(name+"/");
            }
        }

        return possibilities;
    }

    void Shell::addAlias(std::string from, std::string to)
    {
        aliases[from] = to;
    }

    void Shell::getFromServer(NodeValue nodeValue)
    {
        auto name = nodeValue.getName();
        auto value = nodeValue.value;

        if (auto val = Node::asBool(value)) {
            val->value = client->getBool(name);
        } else if (auto val = Node::asInt(value)) {
            val->value = client->getInt(name);
        } else if (auto val = Node::asFloat(value)) {
            val->value = client->getFloat(name);
        } else if (auto val = Node::asString(value)) {
            val->value = client->getStr(name);
        }
    }

    void Shell::setToServer(NodeValue nodeValue)
    {
        auto name = nodeValue.getName();
        auto value = nodeValue.value;

        if (auto val = Node::asBool(value)) {
            client->setBool(name, val->value);
        } else if (auto val = Node::asInt(value)) {
            client->setInt(name, val->value);
        } else if (auto val = Node::asFloat(value)) {
            client->setFloat(name, val->value);
        } else if (auto val = Node::asString(value)) {
            client->setStr(name, val->value);
        }
    }

    void Shell::setFromString(NodeValue nodeValue, std::string str)
    {
        auto name = nodeValue.getName();
        auto value = nodeValue.value;

        if (auto val = Node::asBool(value)) {
            val->value = (str != "0" && str != "false");
        } else if (auto val = Node::asInt(value)) {
            val->value = atoi(str.c_str());
        } else if (auto val = Node::asFloat(value)) {
            val->value = atof(str.c_str());
        } else if (auto val = Node::asString(value)) {
            val->value = str;
        }

        setToServer(nodeValue);
    }
}
