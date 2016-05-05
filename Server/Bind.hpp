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
         * Enum for Push/Pull policy
         */
        enum Policy {
            PushAndPull,
            PushOnly,
            PullOnly,
        };

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
        std::unique_ptr<ValueBuilderBool> bindNew(
            const std::string& name, bool& var,
            Policy policy = PushAndPull);
        std::unique_ptr<ValueBuilderInt> bindNew(
            const std::string& name, int& var,
            Policy policy = PushAndPull);
        std::unique_ptr<ValueBuilderInt> bindNew(
            const std::string& name, long& var,
            Policy policy = PushAndPull);
        std::unique_ptr<ValueBuilderFloat> bindNew(
            const std::string& name, float& var,
            Policy policy = PushAndPull);
        std::unique_ptr<ValueBuilderFloat> bindNew(
            const std::string& name, double& var,
            Policy policy = PushAndPull);
        std::unique_ptr<ValueBuilderStr> bindNew(
            const std::string& name, std::string& var,
            Policy policy = PushAndPull);

        /**
         * Register a variable binding
         * with given name and given
         * variable reference.
         * Variable address must not be changed
         * during this instance lifetime.
         * if value name does not exist, throw
         * std::logic_error
         */
        void bind(const std::string& name, bool& var,
            Policy policy = PushAndPull);
        void bind(const std::string& name, int& var,
            Policy policy = PushAndPull);
        void bind(const std::string& name, long& var,
            Policy policy = PushAndPull);
        void bind(const std::string& name, float& var,
            Policy policy = PushAndPull);
        void bind(const std::string& name, double& var,
            Policy policy = PushAndPull);
        void bind(const std::string& name, std::string& var,
            Policy policy = PushAndPull);

        /**
         * Register given member method and create a new
         * command with given name and description of
         * generic type.
         * Object instance reference is given (to call 
         * the member function)
         * If default arguments are provided, defaultArgs
         * contains the same number of string than the 
         * member number of parameters. Default value
         * are given in string format or empty string "".
         */
        template <typename T, typename Ret, typename ... Args>
        void bindFunc(
            const std::string& name, 
            const std::string& comment, 
            Ret(T::*func)(Args...), 
            T& self,
            const std::vector<std::string>& defaultArgs = {});

        /**
         * Return the child IONode associated with
         * given prefix 
         */
        const IONode& node() const;
        IONode& node();

        /**
         * Alias to StreamNode::newStream
         * and StreamNode::out
         * using internal prefix child
         */
        void newStream(const std::string& name, 
            const std::string& comment);
        std::ostream& out(const std::string& name);
        
        /**
         * Alias to FrameNode::newFrame
         * and FrameNode::framePush
         * using internal prefix child
         */
        void newFrame(const std::string& name, 
            const std::string& comment,
            size_t width, size_t height, FrameFormat format);
        bool frameIsStreaming(const std::string& name) const;
        void framePush(const std::string& name, 
            unsigned char* data, size_t size);

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
            Policy policy;
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
         * Create given absolute path if
         * it does not exist
         */
        void createPath(const std::string& path);

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

#include "BindFunction.hpp"

#endif

