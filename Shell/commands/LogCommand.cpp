#include <iostream>
#include "Shell.h"
#include "LogCommand.h"
#include "CSV.h"
#include "NodePool.h"
#include "Stream.h"

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
        return "log <filename> [param1 [param2 [param3 ...]]]";
    }

    void LogCommand::process(std::vector<std::string> args)
    {
        if (args.size() < 2) {
            errorUsage();
        } else {
            NodePool pool = shell->getPool(args, 1);

            CSV csv;
            csv.open(args[0]);
            pool.setCallback(std::bind(&LogCommand::update, this, &csv, _1));
            shell->streamWait(&pool);
            csv.close();
        }
    }

    void LogCommand::update(CSV *csv, NodePool *pool)
    {
        csv->push("t", pool->timestamp);
        for (auto val : *pool) {
            csv->push(val.value->name, Node::toNumber(val.value));
        }
        csv->newLine();
    }
}
