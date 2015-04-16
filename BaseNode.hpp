#ifndef RHIO_BASENODE_HPP
#define RHIO_BASENODE_HPP

#include <string>
#include <functional>

namespace RhIO {

/**
 * BaseNode
 *
 * Code factorisation of
 * forward function for derive Node classes
 *
 * Forward function is needed for specific Node
 * to call themself across the tree hierarchy
 * controled by IONode
 */
template <typename T>
class BaseNode
{
    public:

        /**
         * Typedef for forward function 
         * to be defined by IONode
         *
         * name is the asked relative path element in tree.
         * If given name is referring to this Node, nullptr
         * is returned. 
         * If given name is referring to a child Node, a pointer
         * to this Node is returned and newName is set to the 
         * given name converted relatively to returned Node.
         * If given name is invalid, throw logic_error exception unless
         * createBranch is true and missing Node are created.
         */
        typedef std::function<T*(
            const std::string& name, std::string& newName, 
            bool createBranch)> ForwardFunc;
        
        /**
         * Initialize and store the forward function
         */
        BaseNode(ForwardFunc func) :
            forwardFunc(func)
        {
        }

    protected:

        /**
         * Hold forward function for derived class
         */
        ForwardFunc forwardFunc;
};

}

#endif

