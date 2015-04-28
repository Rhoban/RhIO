#include <iostream>
#include "Shell.h"
#include "RemoteCommand.h"

namespace RhIO
{
    RemoteCommand::RemoteCommand(std::string fullName_, std::string desc_)
        : fullName(fullName_), desc(desc_), origin(""), name("")
    {
        auto found = fullName.find_last_of("/");
        if (found > 0) {
            origin = fullName.substr(0, found);
            name = fullName.substr(found+1);
        } else {
            name = fullName;
        }
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
