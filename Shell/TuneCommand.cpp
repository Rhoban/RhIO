#include <iostream>
#include "Shell.h"
#include "TuneCommand.h"

namespace RhIO
{
    std::string TuneCommand::getName()
    {
        return "tune";
    }

    std::string TuneCommand::getDesc()
    {
        return "Run the tuner";
    }

    std::string TuneCommand::getUsage()
    {
        return "tune [var1] [var2] [var3]...";
    }

    void TuneCommand::process(std::vector<std::string> args)
    {
        if (!args.size()) {
            Terminal::setColor("red", true);
            std::cout << getUsage();
        } else {

        }
    }
}
