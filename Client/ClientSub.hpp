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
            (const std::string& name, long timestamp, bool val)> 
            StreamBoolHandler;
        typedef std::function<void
            (const std::string& name, long timestamp, long val)> 
            StreamIntHandler;
        typedef std::function<void
            (const std::string& name, long timestamp, double val)> 
            StreamFloatHandler;
        typedef std::function<void
            (const std::string& name, long timestamp, const std::string& val)> 
            StreamStrHandler;
        
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
         * type Bool, Int, Float, Str
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

