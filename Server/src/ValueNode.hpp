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
        int64_t getInt(const std::string& name) const;
        double getFloat(const std::string& name) const;
        const std::string& getStr(const std::string& name) const;

        /**
         * Real time lock free version of values 
         * getters with limited features. 
         * Only direct child value access 
         * (no sub child forward) is supported.
         * No concurrent creation of new values on 
         * the same child node is also assumed.
         */
        bool getRTBool(const std::string& name) const;
        int64_t getRTInt(const std::string& name) const;
        double getRTFloat(const std::string& name) const;

        /**
         * Values setters for each type
         * associated with given relative name 
         * from this Node
         * Optional time_point can be given if the value
         * was update before now
         * On callback is called of 
         * noCallblack is true
         * Throw logic_error exception if asked values name
         * does not exist
         */
        void setBool(const std::string& name, bool val,
            bool noCallblack = false,
            std::chrono::steady_clock::time_point timestamp 
            = std::chrono::steady_clock::now());
        void setInt(const std::string& name, int64_t val,
            bool noCallblack = false,
            std::chrono::steady_clock::time_point timestamp 
            = std::chrono::steady_clock::now());
        void setFloat(const std::string& name, double val,
            bool noCallblack = false,
            std::chrono::steady_clock::time_point timestamp 
            = std::chrono::steady_clock::now());
        void setStr(const std::string& name, const std::string& val,
            bool noCallblack = false,
            std::chrono::steady_clock::time_point timestamp 
            = std::chrono::steady_clock::now());

        /**
         * Real time lock free version of values 
         * setters with limited features. 
         * Only direct child value access 
         * (no sub child forward) is supported.
         * Callback call is not supported.
         * No concurrent creation of new values on 
         * the same child node is also assumed.
         */
        void setRTBool(const std::string& name, bool val,
            std::chrono::steady_clock::time_point timestamp 
            = std::chrono::steady_clock::now());
        void setRTInt(const std::string& name, int64_t val,
            std::chrono::steady_clock::time_point timestamp 
            = std::chrono::steady_clock::now());
        void setRTFloat(const std::string& name, double val,
            std::chrono::steady_clock::time_point timestamp 
            = std::chrono::steady_clock::now());

        /**
         * Real time lock free and atomically add or subtract 
         * given value to a given integral value.
         * Return stored value before the operation is applied.
         * Only direct child value access 
         * (no sub child forward) is supported.
         * Callback call is not supported.
         * No concurrent creation of new values on 
         * the same child node is also assumed.
         * Bounds checks are skipped.
         */
        int64_t addRTInt(const std::string& name, int64_t val,
            std::chrono::steady_clock::time_point timestamp 
            = std::chrono::steady_clock::now());
        int64_t subRTInt(const std::string& name, int64_t val,
            std::chrono::steady_clock::time_point timestamp 
            = std::chrono::steady_clock::now());

        /**
         * Real time lock free and atomically negate a boolean value.
         * Return stored value before the operation is applied.
         * Only direct child value access 
         * (no sub child forward) is supported.
         * Callback call is not supported.
         * No concurrent creation of new values on 
         * the same child node is also assumed.
         */
        bool toggleRTBool(const std::string& name,
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
         * Set a callback function that will be called
         * when the value is updated. The given function
         * will have the new value as argument.
         * CALLBACK FUNCTION MUST NOT CALL ANY 
         * OTHER RHIO FUNCTION (DEADLOCK).
         */
        void setCallbackBool(const std::string& name, 
            std::function<void(bool)> func);
        void setCallbackInt(const std::string& name, 
            std::function<void(int64_t)> func);
        void setCallbackFloat(const std::string& name, 
            std::function<void(double)> func);
        void setCallbackStr(const std::string& name, 
            std::function<void(std::string)> func);

        /**
         * Values structure getters for each type
         * associated with given relative name 
         * from this Node
         * Throw logic_error exception if asked values name
         * does not exist
         * READ VALUE IS NOT THREAD SAFE (use only for meta information)
         */
        const ValueBool& getValueBool(const std::string& name) const;
        const ValueInt& getValueInt(const std::string& name) const;
        const ValueFloat& getValueFloat(const std::string& name) const;
        const ValueStr& getValueStr(const std::string& name) const;

        /**
         * Enable or disable (increase or decrease stream watchers) 
         * for given value name.
         * Check for given name that the streaming value is 
         * enabled with at least one watcher.
         */
        void enableStreamingValue(const std::string& name);
        void disableStreamingValue(const std::string& name);
        void checkStreamingValue(const std::string& name);

        /**
         * Return the relative name list of all registered
         * values for each type
         */
        std::vector<std::string> listValuesBool() const;
        std::vector<std::string> listValuesInt() const;
        std::vector<std::string> listValuesFloat() const;
        std::vector<std::string> listValuesStr() const;

    protected:

        /**
         * Save all values into given path
         */
        void saveValues(const std::string& path);

        /**
         * Parse and load all values into given path
         */
        void loadValues(const std::string& path);

    private:

        /**
         * Contained values map for each types
         */
        std::map<std::string, ValueBool> _valuesBool;
        std::map<std::string, ValueInt> _valuesInt;
        std::map<std::string, ValueFloat> _valuesFloat;
        std::map<std::string, ValueStr> _valuesStr;

        /**
         * Mutex protecting concurrent values creation
         */
        mutable std::mutex _mutex;
};

}

#endif

