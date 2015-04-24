#include <iostream>
#include "Shell.h"
#include "RemoteCommand.h"

namespace RhIO
{
    RemoteCommand::RemoteCommand(std::string origin_, std::string name_, 
            std::string fullName_, std::string desc_)
        : origin(origin_), name(name_), fullName(fullName_), desc(desc_)
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
        auto response = shell->getClient()->call(fullName, args);
        std::cout << response;
        if (response.size() && response[response.size()-1]!='\n') {
            std::cout << std::endl;
        }
    }
}
