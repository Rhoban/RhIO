#pragma once

#include <map>
#include <vector>
#include <RhIOClient.hpp>

namespace RhIO
{
    class Node;

    /**
     * A node value is a representation of a value that belongs to a node
     */
    class NodeValue
    {
        public:
            NodeValue();
            NodeValue(Node *node_, ValueBase *value_);

            /**
             * Pointer to the corresponding ValueBase
             */
            ValueBase *value;

            /**
             * Node that this value belongs to
             */
            Node *node;

            /**
             * Returns the full name of the node, this can be used to
             * set or get the node with the server
             */
            std::string getName();
    };

    class NodeItem
    {
        public:
            Node *node;
            std::string name;
            std::string desc;

            std::string getPath();
            std::string getName();
    };

    class NodeCommand : public NodeItem
    {
    };

    class NodeStream : public NodeItem
    {
    };
    
    class NodeFrame : public NodeItem
    {
        public:
            FrameFormat format;
    };

    class Node
    {
        public:
            Node(ClientReq *client, std::string Path);
            ~Node();

            /**
             * Getting the path of the node
             */
            std::string getPath();

            /**
             * Accessing children
             */
            Node *getChild(std::string name, bool loadIt=true);
            Node *getParent();
            NodeValue getNodeValue(std::string name);
            std::vector<NodeCommand> getCommands();
            std::vector<NodeStream> getStreams();
            std::vector<NodeFrame> getFrames();
            std::vector<std::string> getChildren();

            /**
             * Getting all nodes
             *
             * /!\ Warning: the NodeValue will contains pointers to the elements
             * that are in the above vectors, you should not change these vectors
             * while iterating on this. /!\
             */
            std::vector<NodeValue> getAll();
    
            /**
             * Helpers to get a value as corresponding subtype, returns NULL
             * if the type is not an instance (can be used to test and/or access
             * fields)
             */
            static ValueBool *asBool(ValueBase *value);
            static ValueInt *asInt(ValueBase *value);
            static ValueFloat *asFloat(ValueBase *value);
            static ValueStr *asString(ValueBase *value);

            /**
             * Converts a value base to a string representing it
             */
            static std::string toString(ValueBase *value);

            /**
             * Converts a value to a number representing it
             */
            static double toNumber(ValueBase *value);

            /**
             * Getting a string corresponding to the typpe of the value
             */
            static std::string getType(ValueBase *value);

            /**
             * Tells if the value has changed (if value is different from persisted
             * value)
             */
            static bool isDiff(ValueBase *value);

            /**
             * Getting a string representing the persisted value of a base
             */
            static std::string persistedToString(ValueBase *value);

        protected:
            /**
             * All the values of the node and its children
             */
            std::vector<ValueBool> bools;
            std::vector<ValueInt> ints;
            std::vector<ValueFloat> floats;
            std::vector<ValueStr> strings;
            std::map<std::string, Node*> children;
            std::vector<NodeCommand> commands;
            std::vector<NodeStream> streams;
            std::vector<NodeFrame> frames;
            
            /**
             * Parent node, NULL if root
             */
            Node *parent;
            std::string name, slashed;
            ClientReq *client;
    };
}
