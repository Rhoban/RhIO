#ifndef RHIO_STREAMNODE_HPP
#define RHIO_STREAMNODE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <mutex>

#include "BaseNode.hpp"
#include "Stream.hpp"

namespace RhIO {

/**
 * StreamNode
 *
 * Implement output stream feature for IONode
 */
class StreamNode: public BaseNode<StreamNode>
{
    public:
        
        /**
         * Inherit BaseNode constructor
         */
        using BaseNode<StreamNode>::BaseNode;

        /**
         * Custom assignement operator
         * (because std::mutex is non copyable)
         */
        StreamNode& operator=(const StreamNode& node);

        /**
         * Return true if given stream name is registered
         */
        bool streamExist(const std::string& name) const;

        /**
         * Return stream textual description
         * Throw std::logic_error if given name does not
         * exist
         */
        std::string streamDescription(const std::string& name) const;
        
        /**
         * Enable or disable (increase or decrease stream watchers) 
         * for given stream name
         */
        void enableStreamingStream(const std::string& name);
        void disableStreamingStream(const std::string& name);

        /**
         * Return the std::ostream output stream instance
         * of given relative stream name.
         * Throw std::logic_error if given name does not
         * exist
         */
        std::ostream& out(const std::string& name);

        /**
         * Register a new stream with given name and
         * textual description
         */
        void newStream(const std::string& name,
            const std::string& comment);
        
        /**
         * Return the relative name list 
         * of all registered streams
         */
        std::vector<std::string> listStreams() const;

    private:

        /**
         * Container map for output streams instance
         * and textual description
         */
        std::map<std::string, Stream> _streams;
        
        /**
         * Mutex protecting concurent stream creation
         */
        mutable std::mutex _mutex;
};

}

#endif

