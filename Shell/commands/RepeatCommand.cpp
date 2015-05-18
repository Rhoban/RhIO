#include <iostream>
#include "Shell.h"
#include "RepeatCommand.h"
#include "RemoteCommand.h"
#include <unistd.h>

namespace RhIO
{
    std::string RepeatCommand::getName()
    {
        return "repeat";
    }

    std::string RepeatCommand::getDesc()
    {
        return "Repeats a command";
    }

    void RepeatCommand::process(std::vector<std::string> args)
    {
        std::string command = "";
        for (auto part : args) {
            command += part;
            command += " ";
        }

        while (!shell->hasInput()) {
            system("clear");
            shell->parse(command);

            std::this_thread::sleep_for(
                std::chrono::milliseconds(100));
        }
    }
}
