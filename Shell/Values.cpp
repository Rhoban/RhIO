#include <iostream>
#include <sstream>
#include "Values.h"
#include "Shell.h"

namespace RhIO
{       
    Values::Values(ClientReq *client, std::string path)
    {
        std::string slashed = path;
        if (path != "") {
            slashed += "/";
        }

#define GET_CHILDREN(type, vect)                                 \
        for (auto name : client->listValues ## type (path)) {    \
            auto fullName = slashed+name;                       \
            auto meta = client->metaValue ## type (fullName);    \
            meta.value = client->get ## type (fullName);         \
            meta.name = name;                                    \
            vect.push_back(meta);                                \
        }

        GET_CHILDREN(Bool, bools)
        GET_CHILDREN(Float, floats)
        GET_CHILDREN(Int, ints)
        GET_CHILDREN(Str, strings)

        // Getting childrens
        for (auto name : client->listChildren(path)) {
            children[name] = new Values(client, slashed+name);
        }
    }

    Values::~Values()
    {
        for (auto entry : children) {
            delete entry.second;
        }
    }
            
    Values *Values::getChild(std::string name)
    {
        if (children.count(name)) {
            return children[name];
        }
        
        return NULL;
    }
            
    ValueBase *Values::getValue(std::string name)
    {
        auto all = getAll();

        for (auto entry : all) {
            if (entry->name == name) {
                return entry;
            }
        }

        return NULL;
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

    std::string Values::toString(Shell *shell, ValueBase *value)
    {
        auto client = shell->getClient();
        auto name = shell->getPath() + "/" + value->name;

        if (auto val = asBool(value)) {
            auto v = client->getBool(name);
            return v ? "true" : "false";
        } else if (auto val = asInt(value)) {
            auto v = client->getInt(name);
            std::stringstream ss;
            ss << v;
            return ss.str();
        } else if (auto val = asFloat(value)) {
            auto v = client->getFloat(name);
            std::stringstream ss;
            ss << v;
            return ss.str();
        } else if (auto val = asString(value)) {
            auto v = client->getStr(name);
            return v;
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
            
    void Values::setFromString(Shell *shell, ValueBase *value, std::string str)
    {
        auto client = shell->getClient();
        auto name = shell->getPath() + "/" + value->name;

        if (asBool(value)) {
            client->setBool(name, (str == "1" || str == "true"));
        } else if (asInt(value)) {
            client->setInt(name, atoi(str.c_str()));
        } else if (asFloat(value)) {
            client->setFloat(name, atof(str.c_str()));
        } else if (asString(value)) {
            client->setStr(name, str);
        } 
    }
}
