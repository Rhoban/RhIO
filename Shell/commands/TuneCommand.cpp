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
        if (!args.size()) {
            errorUsage();
        } else {
            Curse curse;

            for (auto arg : args) {
                auto val = shell->getNodeValue(arg);
                if (val.value == NULL) {
                    Terminal::setColor("red", true);
                    std::cout << "Unknown parameter: " << arg << std::endl;
                    Terminal::clear();
                    return;
                }
                curse.values.push_back(val);
            }

            curse.shell = shell;
            curse.run();
        }
    }
}
