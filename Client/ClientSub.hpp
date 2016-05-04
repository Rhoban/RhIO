#ifndef RHIO_CLIENTSUB_HPP
#define RHIO_CLIENTSUB_HPP

#include <string>
#include <functional>
#include <thread>
#include <mutex>
#include <zmq.hpp>

namespace RhIO {

/**
 * ClientSub
 */
class ClientSub
{
    public:

        /**
         * Typedef for stream custom handlers for each type
         */
        typedef std::function<void
            (const std::string& name, int64_t timestamp, bool val)> 
            StreamBoolHandler;
        typedef std::function<void
            (const std::string& name, int64_t timestamp, int64_t val)> 
            StreamIntHandler;
        typedef std::function<void
            (const std::string& name, int64_t timestamp, double val)> 
            StreamFloatHandler;
        typedef std::function<void
            (const std::string& name, int64_t timestamp, const std::string& val)> 
            StreamStrHandler;
        typedef std::function<void
            (const std::string& name, int64_t timestamp, unsigned char* data, size_t size)> 
            StreamFrameHandler;
        
        /**
         * Initialization with the bind
         * endpoint string
         * Starting subscriber thread
         */
        ClientSub(const std::string& endpoint);

        /**
         * Stop running streaming thread
         */
        ~ClientSub();

        /**
         * Setup custom handler for value streaming of each
         * type Bool, Int, Float, Str, Stream
         * Default argument is an empty handler
         */
        void setHandlerBool(
            StreamBoolHandler handler = StreamBoolHandler());
        void setHandlerInt(
            StreamIntHandler handler = StreamIntHandler());
        void setHandlerFloat(
            StreamFloatHandler handler = StreamFloatHandler());
        void setHandlerStr(
            StreamStrHandler handler = StreamStrHandler());
        void setHandlerStream(
            StreamStrHandler handler = StreamStrHandler());
        void setHandlerFrame(
            StreamFrameHandler handler = StreamFrameHandler());

    private:
        
        /**
         * Mutex protecting update on
         * handler function
         */
        std::mutex _mutex;

        /**
         * False if subscriber thread must be stop
         */
        bool _isContinue;

        /**
         * Handle callback for stream values
         */
        StreamBoolHandler _handlerBool;
        StreamIntHandler _handlerInt;
        StreamFloatHandler _handlerFloat;
        StreamStrHandler _handlerStr;
        StreamStrHandler _handlerStream;
        StreamFrameHandler _handlerFrame;
        
        /**
         * Receiver thread
         */
        std::thread _thread;

        /**
         * Receiver thread main loop
         */
        void subscriberThread(const std::string& endpoint);
};

}

#endif

