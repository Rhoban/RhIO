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
            NodePool pool;
            for (int k=1; k<args.size(); k++) {
                auto val = shell->getNodeValue(args[k]);
                if (val.value == NULL) {
                    Terminal::setColor("red", true);
                    std::cout << "Unknown parameter: " << args[k] << std::endl;
                    Terminal::clear();
                    return;
                }
                pool.push_back(val);
            }

            CSV csv;
            csv.open(args[0]);
            pool.setCallback(std::bind(&LogCommand::update, this, &csv, _1));
            shell->getStream()->addPool(&pool);
            std::string line;
            std::getline(std::cin, line);
            shell->getStream()->removePool(&pool);
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
