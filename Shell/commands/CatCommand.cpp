#include <iostream>
#include "Shell.h"
#include "CatCommand.h"
#include "CSV.h"
#include "NodePool.h"
#include "StreamManager.h"

using namespace std::placeholders;

namespace RhIO
{
    std::string CatCommand::getName()
    {
        return "cat";
    }

    std::string CatCommand::getDesc()
    {
        return "Cat a stream";
    }

    std::string CatCommand::getUsage()
    {
        return "cat stream [> filename]";
    }

    void CatCommand::process(std::vector<std::string> args)
    {
        if (!args.size()) {
            errorUsage();
        } else {
            os = getStream(args);
            auto nodeStream = shell->getNodeStream(args[0]);
            auto stream = shell->getStream();

            stream->setStreamCallback(std::bind(&CatCommand::update, this, _1, _2));
            shell->wait();
            stream->unsetStreamCallback();
            clearStream();
        }
    }

    void CatCommand::update(std::string name, std::string message)
    {
        *os << "[" << name << "] " << message << std::endl;
        os->flush();
    }
}
