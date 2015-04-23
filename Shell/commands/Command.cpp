#include <sstream>
#include <iostream>
#include "Shell.h"
#include "Command.h"
#include "Terminal.h"

namespace RhIO
{
    Command::~Command()
    {
    }

    void Command::setShell(Shell *shell_)
    {
        shell = shell_;
    }

    std::string Command::getUsage()
    {
        return "";
    }
            
    void Command::errorUsage()
    {
        std::stringstream ss;
        ss << "Usage: " << getUsage();
        throw ss.str();
    }

    Node *Command::getNode(std::vector<std::string> args)
    {
        std::string dir = "";

        if (args.size()) {
            dir = args[0];
        }
        
        Node *node = shell->getNode(dir);
        if (node == NULL) {
            std::stringstream ss;
            ss << "Unable to get node " << dir;
            throw ss.str();
        }

        return node;
    }
}
