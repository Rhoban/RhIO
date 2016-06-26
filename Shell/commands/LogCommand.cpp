#include <iostream>
#include "Shell.h"
#include "LogCommand.h"
#include "CSV.h"
#include "NodePool.h"
#include "StreamManager.h"

using namespace std::placeholders;

namespace RhIO
{
    std::string LogCommand::getName()
    {
        return "log";
    }

    std::string LogCommand::getDesc()
    {
        return "Log values to a csv file";
    }

    std::string LogCommand::getUsage()
    {
        return "log [param1 [param2 [param3 ...]]] [> filename]";
    }

    void LogCommand::process(std::vector<std::string> args)
    {
        auto output = getStream(args);
        NodePool pool = shell->getPool(args);

        CSV csv(output);
        pool.setCallback(std::bind(&LogCommand::update, this, &csv, _1));
        shell->streamWait(&pool, this);
        clearStream();
    }

    void LogCommand::update(CSV *csv, NodePool *pool)
    {
        csv->push("t", pool->timestamp);
        for (auto val : *pool) {
            csv->push(val.getName(), Node::toNumber(val.value));
        }
        csv->newLine();
    }
}
