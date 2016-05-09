#include <iomanip>
#include <sstream>
#include <iostream>
#include "Shell.h"
#include "LsCommand.h"
#include "Node.h"
#include "NodePool.h"

namespace RhIO
{
    std::string LsCommand::getName()
    {
        return "ls";
    }

    std::string LsCommand::getDesc()
    {
        return "Lists the remote entries";
    }

    void LsCommand::process(std::vector<std::string> args)
    {
        auto node = getNode(args);

        // Listing sub directories
        Terminal::setColor("blue", true);
        for (auto name : node->getChildren()) {
            std::cout << name;
            std::cout << "/" << std::endl;
        }
        Terminal::clear();

        // Listing commands
        /*
        for (auto command : node->getCommands()) {
            Terminal::setColor("green", true);
            printf("%-23s", command.c_str());
        
            if (auto shellCommand = shell->getCommand(command)) {
                Terminal::setColor("blue", false);
                std::cout << "desc: ";
                Terminal::clear();
                std::cout << shellCommand->getDesc();
            }
            std::cout << std::endl;
            Terminal::clear();
        }
        */
        
        // Listing streams
        for (auto stream : node->getStreams()) {
            std::cout << std::left;
            Terminal::setColor("darkblue", true);
            std::cout << std::setw(21) << stream.name;
       
            if (stream.desc != "") {
                Terminal::setColor("magenta", false);
                std::cout << stream.desc;
                Terminal::clear();
            }
            std::cout << std::endl;
            Terminal::clear();
        }
        
        // Listing frames
        for (auto frame : node->getFrames()) {
            std::cout << std::left;
            Terminal::setColor("green", true);
            std::cout << std::setw(21) << frame.name;
            
            std::string format = "";
            if (frame.format == FrameFormat::RGB) {
                format = "RGB";
            } else if (frame.format == FrameFormat::BGR) {
                format = "BGR";
            } else if (frame.format == FrameFormat::YUV) {
                format = "YUV";
            }
            
            std::cout << std::left;
            Terminal::setColor("magenta", false);
            std::cout << std::setw(21) << frame.desc;
            
            std::cout << std::left;
            Terminal::setColor("magenta", false);
            std::cout << std::setw(21) << format;
       
            std::cout << std::endl;
            Terminal::clear();
        }

        NodePool pool = shell->poolForNode(node);
        pool.draw();
    }
}
