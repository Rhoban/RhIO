#include <iostream>
#include <sstream>
#include "Values.h"

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
