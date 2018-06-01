#ifndef RHIO_SERVERPUB_HPP
#define RHIO_SERVERPUB_HPP

#include <string>
#include <list>
#include <mutex>
#include <zmq.hpp>
#include "RhIO.hpp"
#include "LockFreeDoubleQueue.hpp"

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
        ServerPub(std::string endpoint = "");

        /**
         * Append to publish buffer value for type
         * Bool, Int, Float, Str with given absolute
         * name and timestamp
         */
        void publishBool(const std::string& name, 
            bool val, int64_t timestamp);
        void publishInt(const std::string& name, 
            int64_t val, int64_t timestamp);
        void publishFloat(const std::string& name, 
            double val, int64_t timestamp);
        void publishStr(const std::string& name, 
            const std::string& val, int64_t timestamp);

        /**
         * Append to publish buffer stream
         * given absolute name, string and timestamp
         */
        void publishStream(const std::string& name,
            const std::string& val, int64_t timestamp);

        /**
         * Append to publish buffer frame
         * given absolute name, timestamp,
         * frame data and size.
         * The data is immediatly copied.
         */
        void publishFrame(const std::string& name,
            size_t width, size_t height,
            unsigned char* data, size_t size,
            int64_t timestamp);

        /**
         * Switch values buffer and publish to Client
         * all registered values in former writing buffer.
         */
        void sendToClient();

    private:

        /**
         * Structure for typed values 
         * to publish
         */
        template <typename T>
        struct PubValue {
            std::string name;
            T value;
            int64_t timestamp;
        };

        /**
         * Typedef for typed values
         */
        typedef PubValue<bool> PubValBool;
        typedef PubValue<int64_t> PubValInt;
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
         * Lock free double buffer for RT publish
         * of boll, int, float and str values and streams
         */
        LockFreeDoubleQueue<PubValBool> _bufferBool;
        LockFreeDoubleQueue<PubValInt> _bufferInt;
        LockFreeDoubleQueue<PubValFloat> _bufferFloat;
        LockFreeDoubleQueue<PubValStr> _bufferStr;
        LockFreeDoubleQueue<PubValStr> _bufferStream;

        /**
         * If true, the external writing buffer
         * is the 1 and the outputing network buffer is the 2.
         * If false, the external writing buffer 
         * is the 2 and the outputing network buffer is the 1.
         */
        bool _isWritingTo1;

        /**
         * First double buffer for frame publishing
         */
        std::list<zmq::message_t> _queue1Frame;

        /**
         * Second double buffer for frame publishing
         */
        std::list<zmq::message_t> _queue2Frame;

        /**
         * Mutex protecting access to frame double buffer
         */
        std::mutex _mutexQueueFrame;

        /**
         * Swap double buffer for publishing values
         */
        void swapBuffer();
};

}

#endif

