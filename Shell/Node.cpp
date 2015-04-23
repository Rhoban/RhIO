#include <iostream>
#include <sstream>
#include "Node.h"
#include "Shell.h"

namespace RhIO
{       
    NodeValue::NodeValue(Node *node_, ValueBase *value_)
        : node(node_), value(value_)
    {
    }

    std::string NodeValue::getName()
    {
        return node->getPath() + "/" + value->name;
    }

    Node::Node(ClientReq *client, std::string path)
        : name(""), parent(NULL)
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
            children[name] = new Node(client, slashed+name);
            children[name]->name = name;
            children[name]->parent = this;
        }
    }

    std::string Node::getPath()
    {
        std::string path = "";
        if (parent != NULL) {
            path += parent->getPath();
            if (path != "") {
                path += "/";
            }
        }
        path += name;

        return path;
    }

    Node::~Node()
    {
        for (auto entry : children) {
            delete entry.second;
        }
    }
            
    Node *Node::getChild(std::string name)
    {
        if (children.count(name)) {
            return children[name];
        }
        
        return NULL;
    }
            
    NodeValue Node::getNodeValue(std::string name)
    {
        auto all = getAll();

        for (auto entry : all) {
            if (entry.value->name == name) {
                return entry;
            }
        }

        return NodeValue(this, NULL);
    }

    std::vector<NodeValue> Node::getAll()
    {
        std::vector<NodeValue> values;

        for (auto &var : bools) values.push_back(NodeValue(this, &var));
        for (auto &var : ints) values.push_back(NodeValue(this, &var));
        for (auto &var : floats) values.push_back(NodeValue(this, &var));
        for (auto &var : strings) values.push_back(NodeValue(this, &var));

        return values;
    }

    ValueBool *Node::asBool(ValueBase *value)
    {
        return dynamic_cast<ValueBool*>(value);
    }

    ValueInt *Node::asInt(ValueBase *value)
    {
        return dynamic_cast<ValueInt*>(value);
    }

    ValueFloat *Node::asFloat(ValueBase *value)
    {
        return dynamic_cast<ValueFloat*>(value);
    }

    ValueStr *Node::asString(ValueBase *value)
    {
        return dynamic_cast<ValueStr*>(value);
    }

    void Node::get(Shell *shell, NodeValue nodeValue)
    {
        auto client = shell->getClient();
        auto name = nodeValue.getName();
        auto value = nodeValue.value;

        if (auto val = asBool(value)) {
            val->value = client->getBool(name);
        } else if (auto val = asInt(value)) {
            val->value = client->getInt(name);
        } else if (auto val = asFloat(value)) {
            val->value = client->getFloat(name);
        } else if (auto val = asString(value)) {
            val->value = client->getStr(name);
        }
    }

    void Node::set(Shell *shell, NodeValue nodeValue)
    {
        auto client = shell->getClient();
        auto name = nodeValue.getName();
        auto value = nodeValue.value;

        if (auto val = asBool(value)) {
            client->setBool(name, val->value);
        } else if (auto val = asInt(value)) {
            client->setInt(name, val->value);
        } else if (auto val = asFloat(value)) {
            client->setFloat(name, val->value);
        } else if (auto val = asString(value)) {
            client->setStr(name, val->value);
        }
    }

    std::string Node::toString(ValueBase *value)
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
            
    double Node::toNumber(ValueBase *value)
    {
        if (auto val = asBool(value)) {
            return val->value;
        } else if (auto val = asInt(value)) {
            return val->value;
        } else if (auto val = asFloat(value)) {
            return val->value;
        }

        return 0;
    }
            
    std::string Node::getType(ValueBase *value)
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
            
    void Node::setFromString(Shell *shell, NodeValue nodeValue, std::string str)
    {
        auto client = shell->getClient();
        auto name = nodeValue.getName();
        auto value = nodeValue.value;

        if (asBool(value)) {
            client->setBool(name, (str != "0" && str != "false"));
        } else if (asInt(value)) {
            client->setInt(name, atoi(str.c_str()));
        } else if (asFloat(value)) {
            client->setFloat(name, atof(str.c_str()));
        } else if (asString(value)) {
            client->setStr(name, str);
        } 
    }
}
