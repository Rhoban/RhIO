#include <sstream>
#include <iostream>
#include "Shell.h"
#include "TuneCommand.h"
#include "Curse.h"

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
        Curse curse;
        curse.values = shell->getPool(args);
        if (curse.values.size()) {
            curse.shell = shell;
            curse.run();
        } else {
            throw std::string("Nothing to tune");
        }
    }
}
