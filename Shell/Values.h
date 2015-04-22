#pragma once

#include <map>
#include <vector>
#include <ClientReq.hpp>
#include <Value.hpp>

namespace RhIO
{
    class Values
    {
        public:
            Values(ClientReq *client, std::string Path);
            ~Values();

            std::vector<ValueBool> bools;
            std::vector<ValueInt> ints;
            std::vector<ValueFloat> floats;
            std::vector<ValueStr> strings;
            std::map<std::string, Values*> children;

            Values *getChild(std::string name);
    
            static ValueBool *asBool(ValueBase *value);
            static ValueInt *asInt(ValueBase *value);
            static ValueFloat *asFloat(ValueBase *value);
            static ValueStr *asString(ValueBase *value);
            static std::string toString(ValueBase *value);
            static std::string getType(ValueBase *value);

            std::vector<ValueBase*> getAll();
    };
}
