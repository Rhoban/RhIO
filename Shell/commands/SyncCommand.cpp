#include <iostream>
#include "Shell.h"
#include "SyncCommand.h"

namespace RhIO
{
    std::string SyncCommand::getName()
    {
        return "sync";
    }

    std::string SyncCommand::getDesc()
    {
        return "Synchronize the local tree with remote one";
    }

    void SyncCommand::process(std::vector<std::string> args)
    {
        shell->sync();
    }
}
