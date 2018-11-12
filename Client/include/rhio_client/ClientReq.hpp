#ifndef RHIO_CLIENTREQ_HPP
#define RHIO_CLIENTREQ_HPP

#include <vector>
#include <string>
#include <zmq.hpp>
#include "rhio_common/DataBuffer.hpp"
#include "rhio_common/Protocol.hpp"
#include "rhio_common/Value.hpp"
#include "rhio_common/Stream.hpp"
#include "rhio_common/Frame.hpp"

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
         * Return the list of available commands on 
         * a given absolute node name
         */
        std::vector<std::string> listCommands
            (const std::string& name);

        /**
         * Return the absolute name list of all
         * registered commands in global tree
         */
        std::vector<std::string> listAllCommands();

        /**
         * Return the textual command description of 
         * given absolute command name
         */
        std::string commandDescription(const std::string& name);

        /**
         * Call the given server side absolute name command
         * with given arguments list and return the string
         * call result
         */
        std::string call(const std::string& name, 
            const std::vector<std::string>& arguments);

        /**
         * Ask and return the value of given 
         * absolute name for each type
         */
        bool getBool(const std::string& name);
        int64_t getInt(const std::string& name);
        double getFloat(const std::string& name);
        std::string getStr(const std::string& name);

        /**
         * Update with given values the given
         * absolute value name
         */
        void setBool(const std::string& name, bool val);
        void setInt(const std::string& name, int64_t val);
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

        /**
         * Enable and disable streaming for given
         * absolute value name
         * (Increment and decrement stream watchers number).
         * Check streaming insure that given value name
         * streaming is enabled with at least one watchers
         * (use in case of server restart).
         */
        void enableStreamingValue(const std::string& name);
        void disableStreamingValue(const std::string& name);
        void checkStreamingValue(const std::string& name);

        /**
         * Return the list of available streams on 
         * a given absolute node name
         */
        std::vector<std::string> listStreams
            (const std::string& name);

        /**
         * Retrieve textual description for given absolute
         * stream name.
         * Absolute name, comment, and timestamp
         */
        std::string streamDescription(const std::string& name);
        
        /**
         * Return the list of available frames on 
         * a given absolute node name
         */
        std::vector<std::string> listFrames
            (const std::string& name);

        /**
         * Retrieve frame meta information 
         * with given absolute name.
         */
        Frame metaValueFrame(const std::string& name);

        /**
         * Enable and disable streaming for given
         * absolute stream name
         * (Increment and decrement stream watchers number).
         * Check streaming insure that given value name
         * streaming is enabled with at least one watchers
         * (use in case of server restart).
         */
        void enableStreamingStream(const std::string& name);
        void disableStreamingStream(const std::string& name);
        void checkStreamingStream(const std::string& name);

        /**
         * Enable and disable streaming for given
         * absolute frame name
         * (Increment and decrement stream watchers number).
         * Check streaming insure that given value name
         * streaming is enabled with at least one watchers
         * (use in case of server restart).
         */
        void enableStreamingFrame(const std::string& name);
        void disableStreamingFrame(const std::string& name);
        void checkStreamingFrame(const std::string& name);

        /**
         * Save and Load into given absolute node name
         * configuration from given server side directory
         */
        void save(const std::string& name, 
            const std::string& serverPath);
        void load(const std::string& name, 
            const std::string& serverPath);

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

