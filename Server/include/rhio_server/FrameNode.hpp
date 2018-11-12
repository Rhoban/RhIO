#ifndef RHIO_FRAMENODE_HPP
#define RHIO_FRAMENODE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <mutex>

#include "rhio_server/BaseNode.hpp"
#include "rhio_common/Frame.hpp"

namespace RhIO {

/**
 * FrameNode
 *
 * Implemennt frame image streaming 
 * for vision display feature for IONode
 */
class FrameNode: public BaseNode<FrameNode>
{
    public:
        
        /**
         * Inherit BaseNode constructor
         */
        using BaseNode<FrameNode>::BaseNode;
        
        /**
         * Custom assignement operator
         * (because std::mutex is non copyable)
         */
        FrameNode& operator=(const FrameNode& node);

        /**
         * Return true if given frame name is registered
         */
        bool frameExist(const std::string& name) const;

        /**
         * Return true if the given frame name is 
         * currently streamed and watched by clients.
         */
        bool frameIsStreaming(const std::string& name) const;

        /**
         * Return frame meta information 
         * of given frame name
         */
        const Frame& getFrame(const std::string& name) const;

        /**
         * Send to watchers the raw frame with 
         * given name with given data of given size.
         * Frame width and height size are also given.
         * The given data is immediatly copied.
         */
        void framePush(const std::string& name, 
            size_t width, size_t height,
            unsigned char* data, size_t size,
            std::chrono::steady_clock::time_point timestamp 
            = std::chrono::steady_clock::now());
        
        /**
         * Enable or disable (increase or decrease watchers
         * count) for given frame stream name.
         * Check for given name that the streaming frame is 
         * enabled with at least one watcher.
         */
        void enableStreamingFrame(const std::string& name);
        void disableStreamingFrame(const std::string& name);
        void checkStreamingFrame(const std::string& name);
        
        /**
         * Register a new frame with given name,
         * textual description and format
         */
        void newFrame(const std::string& name,
            const std::string& comment, 
            FrameFormat format);

        /**
         * Return the relative name list 
         * of all registered frames
         */
        std::vector<std::string> listFrames() const;

    private:
        
        /**
         * Container map for frame stream instance
         * and textual description
         */
        std::map<std::string, Frame> _frames;
        
        /**
         * Mutex protecting concurent stream creation
         */
        mutable std::mutex _mutex;
};

}

#endif

