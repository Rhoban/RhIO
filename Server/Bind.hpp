#ifndef RHIO_BIND_HPP
#define RHIO_BIND_HPP

#include <vector>
#include <string>
#include "RhIO.hpp"
#include "Value.hpp"
#include "ValueNode.hpp"

namespace RhIO {

/**
 * Bind
 *
 * Object holding variables binding information
 * and link with RhIO values
 */
class Bind
{
    public:

        /**
         * Initialization with child
         * node absolute path to prefix on each call
         * of bind and bindNew
         */
        Bind(const std::string& prefixChild = "");

        /**
         * Create a new value and register a variable
         * for binding for given value name and given
         * variable reference.
         * Variable address must not be changed
         * during this instance lifetime.
         * Return ValueBuilder for setting values meta
         * information on creation
         */
        std::unique_ptr<ValueBuilderBool> 
            bindNew(const std::string& name, bool& var);
        std::unique_ptr<ValueBuilderInt> 
            bindNew(const std::string& name, int& var);
        std::unique_ptr<ValueBuilderInt> 
            bindNew(const std::string& name, long& var);
        std::unique_ptr<ValueBuilderFloat> 
            bindNew(const std::string& name, float& var);
        std::unique_ptr<ValueBuilderFloat> 
            bindNew(const std::string& name, double& var);
        std::unique_ptr<ValueBuilderStr> 
            bindNew(const std::string& name, std::string& var);

        /**
         * Register a variable binding
         * with given name and given
         * variable reference.
         * Variable address must not be changed
         * during this instance lifetime.
         * if value name does not exist, throw
         * std::logic_error
         */
        void bind(const std::string& name, bool& var);
        void bind(const std::string& name, int& var);
        void bind(const std::string& name, long& var);
        void bind(const std::string& name, float& var);
        void bind(const std::string& name, double& var);
        void bind(const std::string& name, std::string& var);

        /**
         * Return the child associated with
         * given prefix 
         */
        const IONode& child() const;
        IONode& child();

        /**
         * Alias to StreamNode::newStream
         * and StreamNode::out
         * using internal prefix child
         */
        void newStream(const std::string& name, 
            const std::string& comment);
        std::ostream& out(const std::string& name);

        /**
         * Import RhIO global values to
         * binded pointer
         */
        void pull();

        /**
         * Export binded pointer to global
         * RhIO values
         */
        void push();

    private:

        /**
         * Structure holding binding
         * information
         * Value relative name, node
         * pointer and value pointer
         */
        template <typename T>
        struct BindInfo {
            std::string name;
            IONode* node;
            T* ptr;
        };

        /**
         * Absolute name prefix to add
         * before each bind name
         */
        std::string _prefix;

        /**
         * Binded information container
         * for each type
         */
        std::vector<BindInfo<bool>> _bindsBool;
        std::vector<BindInfo<int>> _bindsInt;
        std::vector<BindInfo<long>> _bindsLong;
        std::vector<BindInfo<float>> _bindsFloat;
        std::vector<BindInfo<double>> _bindsDouble;
        std::vector<BindInfo<std::string>> _bindsStr;

        /**
         * Extract absolute child name and
         * return node pointer from
         * given value absolute name
         */
        IONode* getChildPtr(const std::string& name) const;
        
        /**
         * Extract relative value name from
         * given value absolute name
         */
        std::string relativeName(const std::string& name) const;
};

}

#endif

