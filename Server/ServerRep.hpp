#ifndef RHIO_SERVERREP_HPP
#define RHIO_SERVERREP_HPP

#include <string>
#include <thread>
#include <zmq.hpp>
#include "RhIO.hpp"
#include "IONode.hpp"
#include "DataBuffer.hpp"

namespace RhIO {

/**
 * ServerRep
 *
 * Implement Client request answer
 * using ZMQ network library
 */
class ServerRep
{
    public:

        /**
         * Initialization with the bind
         * endpoint string
         */
        ServerRep(const std::string& 
            endpoint = std::string("tcp://*:") + ServerRepPort);

        /**
         * Wait for next Client request
         * and reply it
         */
        void handleRequest();

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
         * Reply server thread
         */
        std::thread _serverThread;

        /**
         * Implement MsgAskChildren reply (MsgListNames)
         */
        void listChildren(DataBuffer& buffer);

        /**
         * Implement MsgAskValues Bool, Int, Float and Str
         * (MsgListNames)
         */
        void listValuesBool(DataBuffer& buffer);
        void listValuesInt(DataBuffer& buffer);
        void listValuesFloat(DataBuffer& buffer);
        void listValuesStr(DataBuffer& buffer);

        /**
         * Implement MsgGet for Bool, Int, Float, Str
         * (MsgVal)
         */
        void getBool(DataBuffer& buffer);
        void getInt(DataBuffer& buffer);
        void getFloat(DataBuffer& buffer);
        void getStr(DataBuffer& buffer);

        /**
         * Implement MsgSet for Bool, Int, Float, Str
         * (MsgSetOk)
         */
        void setBool(DataBuffer& buffer);
        void setInt(DataBuffer& buffer);
        void setFloat(DataBuffer& buffer);
        void setStr(DataBuffer& buffer);

        /**
         * Implement MsgAskMeta for Bool, Int, Float, Str
         * (MsgValMeta)
         */
        void valMetaBool(DataBuffer& buffer);
        void valMetaInt(DataBuffer& buffer);
        void valMetaFloat(DataBuffer& buffer);
        void valMetaStr(DataBuffer& buffer);

        /**
         * Implement MsgEnableStreamingValue and MsgDisableStreamingValue
         * (MsgStreamingOK)
         */
        void enableStreamingValue(DataBuffer& buffer);
        void disableStreamingValue(DataBuffer& buffer);

        /**
         * Implement MsgAskSave ans MsgAskLoad (MsgPersistOK)
         */
        void save(DataBuffer& buffer);
        void load(DataBuffer& buffer);

        /**
         * Implement MsgAskCommands (MsgListNames)
         */
        void listCommands(DataBuffer& buffer);

        /**
         * Implement MsgAskAllCommands (MsgListNames)
         */
        void listAllCommands(DataBuffer& buffer);

        /**
         * Implement MsgAskCommandDescription (MsgCommandDescription)
         */
        void commandDescription(DataBuffer& buffer);

        /**
         * Implement MsgAskCall (MsgCallResult)
         */
        void callResult(DataBuffer& buffer);

        /**
         * Implement MsgAskStreams (MsgListNames)
         */
        void listStreams(DataBuffer& buffer);

        /**
         * Implement MsgAskDescriptionStream 
         * (MsgDescriptionStream)
         */
        void descriptionStream(DataBuffer& buffer);

        /**
         * Implement MsgError with given error message
         */
        void error(const std::string& msg);

        /**
         * Return the Node mapped with given absolute name.
         * If given name is invalid, nullptr is returned
         * and an error reply is send to the Client.
         */
        RhIO::IONode* getNode(const std::string& name);
};

}

#endif

