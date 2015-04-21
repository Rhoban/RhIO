#ifndef RHIO_CLIENTREQ_HPP
#define RHIO_CLIENTREQ_HPP

#include <vector>
#include <string>
#include <zmq.hpp>
#include "DataBuffer.hpp"
#include "Protocol.hpp"
#include "Value.hpp"

namespace RhIO {

/**
 * ClientReq
 *
 * Implement Client request interface
 * using ZMQ network library
 */
class ClientReq
{
    public:

        /**
         * Initialization with the bind
         * endpoint string
         */
        ClientReq(const std::string& endpoint);

        /**
         * Return the list of children node name
         * of given absolute node name
         */
        std::vector<std::string> listChildren
            (const std::string& name);

        /**
         * Return the list of values name registered
         * into given absolute node name for each types
         */
        std::vector<std::string> listValuesBool
            (const std::string& name);
        std::vector<std::string> listValuesInt
            (const std::string& name);
        std::vector<std::string> listValuesFloat
            (const std::string& name);
        std::vector<std::string> listValuesStr
            (const std::string& name);

        /**
         * Ask and return the value of given 
         * absolute name for each type
         */
        bool getBool(const std::string& name);
        long getInt(const std::string& name);
        double getFloat(const std::string& name);
        std::string getStr(const std::string& name);

        /**
         * Update with given values the given
         * absolute value name
         */
        void setBool(const std::string& name, bool val);
        void setInt(const std::string& name, long val);
        void setFloat(const std::string& name, double val);
        void setStr(const std::string& name, 
            const std::string& val);

        /**
         * Retrieve value meta information for each type 
         * with given absolute name.
         * Only name, comment, min, max, hasMin, hasMax and
         * persisted is set.
         */
        ValueBool metaValueBool(const std::string& name);
        ValueInt metaValueInt(const std::string& name);
        ValueFloat metaValueFloat(const std::string& name);
        ValueStr metaValueStr(const std::string& name);

    private:

        /**
         * ZMQ context
         */
        zmq::context_t _context;

        /**
         * ZMQ socket
         */
        zmq::socket_t _socket;

        /**
         * Call recv on ZMQ socket
         * and throw an error
         * if the receivned message type 
         * is MsgError or is different than 
         * given message type.
         * Return a DataBuffer on received data
         */
        DataBuffer waitReply
            (zmq::message_t& reply, MsgType expectedType);

        /**
         * Throw runtime_error with given error message
         */
        void error(const std::string& msg);

        /**
         * Request the Server with given message type
         * and return result as list of string
         * Factorize listValues ans listChildren
         */
        std::vector<std::string> listNames(MsgType msgType, 
            const std::string& name);
};

}

#endif

