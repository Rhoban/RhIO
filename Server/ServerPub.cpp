#include "ServerPub.hpp"
#include "Protocol.hpp"
#include "DataBuffer.hpp"

namespace RhIO {

ServerPub::ServerPub(const std::string& endpoint) :
    _context(1),
    _socket(_context, ZMQ_PUB),
    _isWritingTo1(true),
    _queue1Bool(),
    _queue1Int(),
    _queue1Float(),
    _queue1Str(),
    _queue2Bool(),
    _queue2Int(),
    _queue2Float(),
    _queue2Str(),
    _mutexQueueBool(),
    _mutexQueueInt(),
    _mutexQueueFloat(),
    _mutexQueueStr()
{
    _socket.bind(endpoint.c_str());
}

void ServerPub::publishBool(const std::string& name, 
    bool val, long timestamp)
{
    std::lock_guard<std::mutex> lock(_mutexQueueBool);
    if (_isWritingTo1) {
        _queue1Bool.push_back({name, val, timestamp});
    } else {
        _queue2Bool.push_back({name, val, timestamp});
    }
}
void ServerPub::publishInt(const std::string& name, 
    long val, long timestamp)
{
    std::lock_guard<std::mutex> lock(_mutexQueueInt);
    if (_isWritingTo1) {
        _queue1Int.push_back({name, val, timestamp});
    } else {
        _queue2Int.push_back({name, val, timestamp});
    }
}
void ServerPub::publishFloat(const std::string& name, 
    double val, long timestamp)
{
    std::lock_guard<std::mutex> lock(_mutexQueueFloat);
    if (_isWritingTo1) {
        _queue1Float.push_back({name, val, timestamp});
    } else {
        _queue2Float.push_back({name, val, timestamp});
    }
}
void ServerPub::publishStr(const std::string& name, 
    const std::string& val, long timestamp)
{
    std::lock_guard<std::mutex> lock(_mutexQueueStr);
    if (_isWritingTo1) {
        _queue1Str.push_back({name, val, timestamp});
    } else {
        _queue2Str.push_back({name, val, timestamp});
    }
}
        
void ServerPub::sendToClient()
{
    //Swap double buffer
    //Later network communication is lock-free
    swapBuffer();

    //Reference on full value buffer to be send
    std::list<PubValBool>& queueBool = 
        (_isWritingTo1) ? _queue2Bool : _queue1Bool;
    std::list<PubValInt>& queueInt = 
        (_isWritingTo1) ? _queue2Int : _queue1Int;
    std::list<PubValFloat>& queueFloat = 
        (_isWritingTo1) ? _queue2Float : _queue1Float;
    std::list<PubValStr>& queueStr = 
        (_isWritingTo1) ? _queue2Str : _queue1Str;

    //Sending values Bool
    while (!queueBool.empty()) {
        //Allocate message data
        zmq::message_t packet(
            sizeof(MsgType) + sizeof(long) 
            + queueBool.front().name.length()
            + sizeof(long) + sizeof(uint8_t));
        DataBuffer pub(packet.data(), packet.size());
        pub.writeType(MsgStreamBool);
        pub.writeStr(queueBool.front().name);
        pub.writeInt(queueBool.front().timestamp);
        pub.writeBool(queueBool.front().value);

        //Send packet
        _socket.send(packet);

        //Pop value
        queueBool.pop_front();
    }
    //Sending values Int
    while (!queueInt.empty()) {
        //Allocate message data
        zmq::message_t packet(
            sizeof(MsgType) + sizeof(long) 
            + queueInt.front().name.length()
            + sizeof(long) + sizeof(long));
        DataBuffer pub(packet.data(), packet.size());
        pub.writeType(MsgStreamInt);
        pub.writeStr(queueInt.front().name);
        pub.writeInt(queueInt.front().timestamp);
        pub.writeInt(queueInt.front().value);

        //Send packet
        _socket.send(packet);

        //Pop value
        queueInt.pop_front();
    }
    //Sending values Float
    while (!queueFloat.empty()) {
        //Allocate message data
        zmq::message_t packet(
            sizeof(MsgType) + sizeof(long) 
            + queueFloat.front().name.length()
            + sizeof(long) + sizeof(double));
        DataBuffer pub(packet.data(), packet.size());
        pub.writeType(MsgStreamFloat);
        pub.writeStr(queueFloat.front().name);
        pub.writeInt(queueFloat.front().timestamp);
        pub.writeFloat(queueFloat.front().value);

        //Send packet
        _socket.send(packet);

        //Pop value
        queueFloat.pop_front();
    }
    //Sending values Str
    while (!queueStr.empty()) {
        //Allocate message data
        zmq::message_t packet(
            sizeof(MsgType) + sizeof(long) 
            + queueStr.front().name.length()
            + sizeof(long) + sizeof(long)
            + queueStr.front().value.length());
        DataBuffer pub(packet.data(), packet.size());
        pub.writeType(MsgStreamStr);
        pub.writeStr(queueStr.front().name);
        pub.writeInt(queueStr.front().timestamp);
        pub.writeStr(queueStr.front().value);

        //Send packet
        _socket.send(packet);

        //Pop value
        queueStr.pop_front();
    }
}
        
void ServerPub::swapBuffer()
{
    //Lock all publisher buffer for all types
    std::lock_guard<std::mutex> lockBool(_mutexQueueBool);
    std::lock_guard<std::mutex> lockInt(_mutexQueueInt);
    std::lock_guard<std::mutex> lockFloat(_mutexQueueFloat);
    std::lock_guard<std::mutex> lockStr(_mutexQueueStr);
    //Swap buffer
    _isWritingTo1 = !_isWritingTo1;
}

}

