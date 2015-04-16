#ifndef RHIO_VALUENODE_HPP
#define RHIO_VALUENODE_HPP

#include <map>
#include <string>
#include <functional>

#include "Value.hpp"
#include "BaseNode.hpp"

namespace RhIO {

/**
 * ValueNode
 *
 * Implement Value features for IONode
 */
class ValueNode : public BaseNode<ValueNode>
{
    public:

        /**
         * Inherit BaseNode constructor
         */
        using BaseNode<ValueNode>::BaseNode;

        /**
         * Return value type of given relative name
         * or NoValue if the given name does not exist
         */
        ValueType getValueType(const std::string& name) const;

        /**
         * Values getters for each type
         * associated with given relative name 
         * from this Node
         * Throw logic_error exception if asked values name
         * does not exist
         */
        bool getBool(const std::string& name) const;
        long getInt(const std::string& name) const;
        double getFloat(const std::string& name) const;
        const std::string& getStr(const std::string& name) const;

        /**
         * Values setters for each type
         * associated with given relative name 
         * from this Node
         * Throw logic_error exception if asked values name
         * does not exist
         */
        void setBool(const std::string& name, bool val);
        void setInt(const std::string& name, long val);
        void setFloat(const std::string& name, double val);
        void setStr(const std::string& name, const std::string& val);

        /**
         * Values structure getters for each type
         * associated with given relative name 
         * from this Node
         * Throw logic_error exception if asked values name
         * does not exist
         */
        const ValueBool& getValueBool(const std::string& name) const;
        const ValueInt& getValueInt(const std::string& name) const;
        const ValueFloat& getValueFloat(const std::string& name) const;
        const ValueStr& getValueStr(const std::string& name) const;

        /**
         * Direct read access to Values
         * container for tree traversal
         */
        const std::map<std::string, ValueBool>& accessValuesBool() const;
        const std::map<std::string, ValueInt>& accessValuesInt() const;
        const std::map<std::string, ValueFloat>& accessValuesFloat() const;
        const std::map<std::string, ValueStr>& accessValuesStr() const;

    private:

        /**
         * Contained values map for each types
         */
        std::map<std::string, ValueBool> _valuesBool;
        std::map<std::string, ValueInt> _valuesInt;
        std::map<std::string, ValueFloat> _valuesFloat;
        std::map<std::string, ValueStr> _valuesStr;
};

}

#endif

