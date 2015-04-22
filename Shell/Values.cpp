#include <sstream>
#include "Values.h"

namespace RhIO
{       
    Values::Values(ClientReq *client, std::string path)
    {
        for (auto name : client->listValuesBool(path)) {
            auto fullName = path+"/"+name;
            auto meta = client->metaValueBool(fullName);
            meta.value = client->getBool(fullName);
            meta.name = name;
            bools.push_back(meta);
        }
        for (auto name : client->listValuesInt(path)) {
            auto fullName = path+"/"+name;
            auto meta = client->metaValueInt(fullName);
            meta.value = client->getInt(fullName);
            meta.name = name;
            ints.push_back(meta);
        }
        for (auto name : client->listValuesFloat(path)) {
            auto fullName = path+"/"+name;
            auto meta = client->metaValueFloat(fullName);
            meta.value = client->getFloat(fullName);
            meta.name = name;
            floats.push_back(meta);
        }
        for (auto name : client->listValuesStr(path)) {
            auto fullName = path+"/"+name;
            auto meta = client->metaValueStr(fullName);
            meta.value = client->getStr(fullName);
            meta.name = name;
            strings.push_back(meta);
        }
    }

    std::vector<ValueBase*> Values::getAll()
    {
        std::vector<ValueBase*> values;

        for (auto &var : bools) values.push_back(&var);
        for (auto &var : ints) values.push_back(&var);
        for (auto &var : floats) values.push_back(&var);
        for (auto &var : strings) values.push_back(&var);

        return values;
    }

    ValueBool *Values::asBool(ValueBase *value)
    {
        return dynamic_cast<ValueBool*>(value);
    }

    ValueInt *Values::asInt(ValueBase *value)
    {
        return dynamic_cast<ValueInt*>(value);
    }

    ValueFloat *Values::asFloat(ValueBase *value)
    {
        return dynamic_cast<ValueFloat*>(value);
    }

    ValueStr *Values::asString(ValueBase *value)
    {
        return dynamic_cast<ValueStr*>(value);
    }

    std::string Values::toString(ValueBase *value)
    {
        if (auto val = asBool(value)) {
            return val->value ? "true" : "false";
        } else if (auto val = asInt(value)) {
            std::stringstream ss;
            ss << val->value;
            return ss.str();
        } else if (auto val = asFloat(value)) {
            std::stringstream ss;
            ss << val->value;
            return ss.str();
        } else if (auto val = asString(value)) {
            return val->value;
        } else {
            return "?";
        }
    }
            
    std::string Values::getType(ValueBase *value)
    {
        if (asBool(value)) {
            return "bool";
        } else if (asInt(value)) {
            return "int";
        } else if (asFloat(value)) {
            return "float";
        } else if (asString(value)) {
            return "string";
        } else {
            return "?";
        }
    }
}
