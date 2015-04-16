#ifndef RHIO_IONODE_HPP
#define RHIO_IONODE_HPP

#include <map>
#include <string>
#include "ValueNode.hpp"

namespace RhIO {

/**
 * Tree name separator
 */
constexpr char separator = '/';

/**
 * IONode
 *
 * TODO
 */
class IONode final : public ValueNode
{
    public:

        /**
         * Initialization
         */
        IONode();

        /**
         * Return Node name
         */
        const std::string& name() const;

        /**
         * Return Node parent and return self
         * if this is the root
         */
        const IONode& parent() const;
        IONode& parent();

        /**
         * Return Root Node
         */
        const IONode& root() const;
        IONode& root();

        /**
         * Return true if given subnode exist
         */
        bool childExist(const std::string& name) const;

        /**
         * Find and return child Node with given relative name
         * Throw logic_error exception if the asked node name
         * does not exist
         */
        const IONode& child(const std::string& name) const;
        IONode& child(const std::string& name);

        /**
         * Direct read access to children Nodes
         * container for tree traversal
         */
        const std::map<std::string, IONode>& accessChildren() const; 
    private:

        /**
         * Node name
         */
        std::string _name;

        /**
         * Pointer to parent Node
         * nullptr if root
         */
        IONode* _parent;

        /**
         * Children Nodes container 
         */
        std::map<std::string, IONode> _children;

        /**
         * Optional prefix to force the subtree to save
         * inside specific config file
         */
        std::string _configFilePrefix;

        /**
         * If given name is referring to this Node, nullptr
         * is returned. 
         * If given name is referring to a child Node, a pointer
         * to this Node is returned and newName is set to the 
         * given name converted relatively to returned Node.
         * If given name is invalid, throw logic_error exception.
         */
        IONode* forwardChildren(
            const std::string& name, std::string& newName);
};

}

#endif

