#include <iostream>
#include "Shell.h"
#include "RemoteCommand.h"

namespace RhIO
{
    RemoteCommand::RemoteCommand(std::string origin_, std::string name_, std::string desc_)
        : origin(origin_), name(name_), desc(desc_)
    {
    }

    std::string RemoteCommand::getName()
    {
        return name;
    }

    std::string RemoteCommand::getDesc()
    {
        return desc;
    }

    std::string RemoteCommand::getOrigin()
    {
        return origin;
    }

    void RemoteCommand::process(std::vector<std::string> args)
    {
        std::cout << shell->getClient()->callCommand(name, args);
    }
}
