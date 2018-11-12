#ifndef RHIO_STREAM_HPP
#define RHIO_STREAM_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <memory>

namespace RhIO {

/**
 * Custom string buffer to catch
 * stream flush operation
 */
struct StreamBuffer : public std::stringbuf
{
    public:

        /**
         * Initialization with stream absolute name
         */
        StreamBuffer(const std::string& name);

        /**
         * Override to catch ostream flush
         */
        virtual int sync() override;
        
        /**
         * The number of registered watcher.
         * Streaming is enabled while at least
         * one watcher is registered
         */
        int64_t _streamWatchers;

    private:

        /**
         * Stream absolute name
         */
        std::string _pwd;
};

/**
 * Stream
 *
 * Hold all Stream
 * internal information
 */
struct Stream
{
    /**
     * Textual description
     */
    std::string comment;
    
    /**
     * External ostream instance
     */
    std::shared_ptr<std::ostream> stream;

    /**
     * Internal custom string buffer
     */
    std::shared_ptr<StreamBuffer> buffer;
};

}

#endif

