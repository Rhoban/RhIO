#pragma once

#include <map>
#include <vector>
#include <ClientReq.hpp>
#include <Value.hpp>

namespace RhIO
{
    class Shell;
    class Node;

    class NodeValue
    {
        public:
            NodeValue(Node *node_, ValueBase *value_);
            Node *node;
            ValueBase *value;

            std::string getName();
    };

    class Node
    {
        public:
            Node(ClientReq *client, std::string Path);
            ~Node();

            std::string getPath();

            std::vector<ValueBool> bools;
            std::vector<ValueInt> ints;
            std::vector<ValueFloat> floats;
            std::vector<ValueStr> strings;
            std::map<std::string, Node*> children;

            Node *getChild(std::string name);
            NodeValue getNodeValue(std::string name);
    
            static ValueBool *asBool(ValueBase *value);
            static ValueInt *asInt(ValueBase *value);
            static ValueFloat *asFloat(ValueBase *value);
            static ValueStr *asString(ValueBase *value);

            static void get(Shell *shell, NodeValue value);
            static void set(Shell *shell, NodeValue value);
            static std::string toString(ValueBase *value);
            static double toNumber(ValueBase *value);
            static std::string getType(ValueBase *value);

            static void setFromString(Shell *shell, NodeValue value, std::string str);

            std::vector<NodeValue> getAll();
            std::string name;
            Node *parent;
    };
}
