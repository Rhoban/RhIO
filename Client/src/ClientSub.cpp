#include <stdexcept>
#include <chrono>
#include "rhio_client/ClientSub.hpp"
#include "rhio_common/Protocol.hpp"
#include "rhio_common/DataBuffer.hpp"

namespace RhIO {
        
ClientSub::ClientSub(const std::string& endpoint) :
    _mutex(),
    _isContinue(true),
    _handlerBool(StreamBoolHandler()),
    _handlerInt(StreamIntHandler()),
    _handlerFloat(StreamFloatHandler()),
    _handlerStr(StreamStrHandler()),
    _handlerStream(StreamStrHandler()),
    _handlerFrame(StreamFrameHandler()),
    //Starting receiver thread
    _thread(&ClientSub::subscriberThread, this, endpoint)
{
}
        
ClientSub::~ClientSub()
{
    //Asking subscriber thread stop
    _isContinue = false;
    //Waiting thread end
    _thread.join();
}

void ClientSub::setHandlerBool(StreamBoolHandler handler)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _handlerBool = handler;
}
void ClientSub::setHandlerInt(StreamIntHandler handler)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _handlerInt = handler;
}
void ClientSub::setHandlerFloat(StreamFloatHandler handler)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _handlerFloat = handler;
}
void ClientSub::setHandlerStr(StreamStrHandler handler)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _handlerStr = handler;
}
void ClientSub::setHandlerStream(StreamStrHandler handler)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _handlerStream = handler;
}
void ClientSub::setHandlerFrame(StreamFrameHandler handler)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _handlerFrame = handler;
}
        
void ClientSub::subscriberThread(const std::string& endpoint)
{
    //ZMQ context
    zmq::context_t context(1);
    //ZMQ socket
    zmq::socket_t socket(context, ZMQ_DISH);
    //Connection to Server
    socket.bind(endpoint.c_str());
    //Subscribing to rhio group
    socket.join("rhio");

    while (_isContinue) {
        //Wait for Server next packet
        zmq::message_t packet;
        if (!socket.recv(&packet, ZMQ_NOBLOCK)) {
            //If no data is available wait a bit
            std::this_thread::sleep_for(
                std::chrono::milliseconds(1));
            continue;
        }
        DataBuffer sub(packet.data(), packet.size());

        //Check empty message
        if (sub.size() == 0) {
            throw std::logic_error(
                "RhIOClient empty server message");
        }
        //Retrieve message type
        MsgType type = (MsgType)sub.readType();
        if (type == MsgStreamBool) {
            //Stream Bool value
            std::string name = sub.readStr();
            int64_t timestamp = sub.readInt();
            bool val = sub.readBool();
            std::lock_guard<std::mutex> lock(_mutex);
            if (_handlerBool) {
                _handlerBool(name, timestamp, val);
            }
            continue;
        } else if (type == MsgStreamInt) {
            //Stream Int value
            std::string name = sub.readStr();
            int64_t timestamp = sub.readInt();
            int64_t val = sub.readInt();
            std::lock_guard<std::mutex> lock(_mutex);
            if (_handlerInt) {
                _handlerInt(name, timestamp, val);
            }
            continue;
        } else if (type == MsgStreamFloat) {
            //Stream Float value
            std::string name = sub.readStr();
            int64_t timestamp = sub.readInt();
            double val = sub.readFloat();
            std::lock_guard<std::mutex> lock(_mutex);
            if (_handlerFloat) {
                _handlerFloat(name, timestamp, val);
            }
            continue;
        } else if (type == MsgStreamStr) {
            //Stream Str value
            std::string name = sub.readStr();
            int64_t timestamp = sub.readInt();
            std::string val = sub.readStr();
            std::lock_guard<std::mutex> lock(_mutex);
            if (_handlerStr) {
                _handlerStr(name, timestamp, val);
            }
            continue;
        } else if (type == MsgStreamStream) {
            //Stream Stream value
            std::string name = sub.readStr();
            int64_t timestamp = sub.readInt();
            std::string val = sub.readStr();
            std::lock_guard<std::mutex> lock(_mutex);
            if (_handlerStream) {
                _handlerStream(name, timestamp, val);
            }
            continue;
        } else if (type == MsgStreamFrame) {
            //Stream Frame value
            std::string name = sub.readStr();
            int64_t timestamp = sub.readInt();
            int64_t width = sub.readInt();
            int64_t height = sub.readInt();
            size_t size;
            unsigned char* data = sub.readData(size);
            std::lock_guard<std::mutex> lock(_mutex);
            if (_handlerFrame) {
                _handlerFrame(name, timestamp, width, height, data, size);
            }
            continue;
        } else {
            throw std::logic_error(
                "RhIOClient invalid stream message type");
        }
    }
}

}

