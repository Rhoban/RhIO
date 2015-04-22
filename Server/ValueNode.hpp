#ifndef RHIO_VALUENODE_HPP
#define RHIO_VALUENODE_HPP

#include <map>
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <chrono>
#include <mutex>

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
         * Custom assignement operator
         * (because std::mutex is non copyable)
         */
        ValueNode& operator=(const ValueNode& node);

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
         * Optional time_point can be given if the value
         * was update before now
         * Throw logic_error exception if asked values name
         * does not exist
         */
        void setBool(const std::string& name, bool val,
            std::chrono::steady_clock::time_point timestamp 
            = std::chrono::steady_clock::now());
        void setInt(const std::string& name, long val,
            std::chrono::steady_clock::time_point timestamp 
            = std::chrono::steady_clock::now());
        void setFloat(const std::string& name, double val,
            std::chrono::steady_clock::time_point timestamp 
            = std::chrono::steady_clock::now());
        void setStr(const std::string& name, const std::string& val,
            std::chrono::steady_clock::time_point timestamp 
            = std::chrono::steady_clock::now());

        /**
         * Declare a new value with given relative name for each type.
         * Subtree Node is created if relative branch path does not exist.
         * ValueBuilder is returned in order to be able to set optional
         * value parameters.
         */
        std::unique_ptr<ValueBuilderBool> newBool(const std::string& name);
        std::unique_ptr<ValueBuilderInt> newInt(const std::string& name);
        std::unique_ptr<ValueBuilderFloat> newFloat(const std::string& name);
        std::unique_ptr<ValueBuilderStr> newStr(const std::string& name);

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
         * Return the relative name list of all registered
         * values for each type
         */
        std::vector<std::string> listValuesBool() const;
        std::vector<std::string> listValuesInt() const;
        std::vector<std::string> listValuesFloat() const;
        std::vector<std::string> listValuesStr() const;

    private:

        /**
         * Contained values map for each types
         */
        std::map<std::string, ValueBool> _valuesBool;
        std::map<std::string, ValueInt> _valuesInt;
        std::map<std::string, ValueFloat> _valuesFloat;
        std::map<std::string, ValueStr> _valuesStr;

        /**
         * Mutex protecting concurent values creation
         */
        mutable std::mutex _mutex;
};

}

#endif

