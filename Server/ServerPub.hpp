#ifndef RHIO_SERVERPUB_HPP
#define RHIO_SERVERPUB_HPP

#include <string>
#include <list>
#include <mutex>
#include <zmq.hpp>
#include "RhIO.hpp"

namespace RhIO {

/**
 * ServerPub
 *
 * Implement Streaming  publisher
 * using ZMQ network library
 */
class ServerPub
{
    public:
        
        /**
         * Initialization with the bind
         * endpoint string
         */
        ServerPub(const std::string& 
            endpoint = std::string("tcp://*:") + ServerPubPort);

        /**
         * Append to publish buffer value for type
         * Bool, Int, Float, Str with given absolute
         * name and timestamp
         */
        void publishBool(const std::string& name, 
            bool val, long timestamp);
        void publishInt(const std::string& name, 
            long val, long timestamp);
        void publishFloat(const std::string& name, 
            double val, long timestamp);
        void publishStr(const std::string& name, 
            const std::string& val, long timestamp);

        /**
         * Append to publish buffer stream
         * given absolute name, string and timestamp
         */
        void publishStream(const std::string& name,
            const std::string& val, long timestamp);

        /**
         * Switch values buffer and publish to Client
         * all registered values in former writing buffer.
         */
        void sendToClient();

    private:

        /**
         * Structure for typed values to 
         * publish
         */
        template <typename T>
        struct PubValue {
            std::string name;
            T value;
            long timestamp;
        };

        /**
         * Typedef for typed values
         */
        typedef PubValue<bool> PubValBool;
        typedef PubValue<long> PubValInt;
        typedef PubValue<double> PubValFloat;
        typedef PubValue<std::string> PubValStr;
        
        /**
         * ZMQ context
         */
        zmq::context_t _context;

        /**
         * ZMQ socket
         */
        zmq::socket_t _socket;

        /**
         * If true, the external writing buffer
         * is the 1 and the outputing network buffer is the 2.
         * If false, the external writing buffer 
         * is the 2 and the outputing network buffer is the 1.
         */
        bool _isWritingTo1;

        /**
         * First double buffer values to
         * publish container
         */
        std::list<PubValBool> _queue1Bool;
        std::list<PubValInt> _queue1Int;
        std::list<PubValFloat> _queue1Float;
        std::list<PubValStr> _queue1Str;
        std::list<PubValStr> _queue1Stream;

        /**
         * Second double buffer values to
         * publish container
         */
        std::list<PubValBool> _queue2Bool;
        std::list<PubValInt> _queue2Int;
        std::list<PubValFloat> _queue2Float;
        std::list<PubValStr> _queue2Str;
        std::list<PubValStr> _queue2Stream;

        /**
         * Mutex protecting external values container
         * from concurent access for each type
         */
        std::mutex _mutexQueueBool;
        std::mutex _mutexQueueInt;
        std::mutex _mutexQueueFloat;
        std::mutex _mutexQueueStr;
        std::mutex _mutexQueueStream;

        /**
         * Swap double buffer for publishing values
         */
        void swapBuffer();
};

}

#endif

