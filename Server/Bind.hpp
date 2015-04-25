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
         * Register a variable binding
         * with given absolute name and given
         * variable reference.
         * Variable address must not be changed
         * during this instance lifetime.
         * if variable does not exist, a new values
         * is created with default parameter
         */
        void bind(const std::string& name, bool& var);
        void bind(const std::string& name, int& var);
        void bind(const std::string& name, long& var);
        void bind(const std::string& name, float& var);
        void bind(const std::string& name, double& var);
        void bind(const std::string& name, std::string& var);

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

