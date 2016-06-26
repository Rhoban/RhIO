#include "Server/ServerPub.hpp"
#include "Common/Protocol.hpp"
#include "Common/DataBuffer.hpp"

namespace RhIO {

ServerPub::ServerPub(const std::string& endpoint) :
    _context(1),
    _socket(_context, ZMQ_PUB),
    _isWritingTo1(true),
    _queue1Bool(),
    _queue1Int(),
    _queue1Float(),
    _queue1Str(),
    _queue1Stream(),
    _queue1Frame(),
    _queue2Bool(),
    _queue2Int(),
    _queue2Float(),
    _queue2Str(),
    _queue2Stream(),
    _queue2Frame(),
    _mutexQueueBool(),
    _mutexQueueInt(),
    _mutexQueueFloat(),
    _mutexQueueStr(),
    _mutexQueueStream(),
    _mutexQueueFrame()
{
    _socket.bind(endpoint.c_str());
}

void ServerPub::publishBool(const std::string& name, 
    bool val, int64_t timestamp)
{
    std::lock_guard<std::mutex> lock(_mutexQueueBool);
    if (_isWritingTo1) {
        _queue1Bool.push_back({name, val, timestamp});
    } else {
        _queue2Bool.push_back({name, val, timestamp});
    }
}
void ServerPub::publishInt(const std::string& name, 
    int64_t val, int64_t timestamp)
{
    std::lock_guard<std::mutex> lock(_mutexQueueInt);
    if (_isWritingTo1) {
        _queue1Int.push_back({name, val, timestamp});
    } else {
        _queue2Int.push_back({name, val, timestamp});
    }
}
void ServerPub::publishFloat(const std::string& name, 
    double val, int64_t timestamp)
{
    std::lock_guard<std::mutex> lock(_mutexQueueFloat);
    if (_isWritingTo1) {
        _queue1Float.push_back({name, val, timestamp});
    } else {
        _queue2Float.push_back({name, val, timestamp});
    }
}
void ServerPub::publishStr(const std::string& name, 
    const std::string& val, int64_t timestamp)
{
    std::lock_guard<std::mutex> lock(_mutexQueueStr);
    if (_isWritingTo1) {
        _queue1Str.push_back({name, val, timestamp});
    } else {
        _queue2Str.push_back({name, val, timestamp});
    }
}

void ServerPub::publishStream(const std::string& name, 
    const std::string& val, int64_t timestamp)
{
    std::lock_guard<std::mutex> lock(_mutexQueueStream);
    if (_isWritingTo1) {
        _queue1Stream.push_back({name, val, timestamp});
    } else {
        _queue2Stream.push_back({name, val, timestamp});
    }
}

void ServerPub::publishFrame(const std::string& name,
    size_t width, size_t height,
    unsigned char* data, size_t size,
    int64_t timestamp)
{
    //Directly allocate message data
    std::lock_guard<std::mutex> lock(_mutexQueueFrame);
    if (_isWritingTo1) {
        _queue1Frame.clear();
        _queue1Frame.push_back(zmq::message_t(
            sizeof(MsgType) 
            + sizeof(int64_t) 
            + name.length()
            + 4*sizeof(int64_t) 
            + size
        ));
        DataBuffer pub(_queue1Frame.back().data(), _queue1Frame.back().size());
        pub.writeType(MsgStreamFrame);
        pub.writeStr(name);
        pub.writeInt(timestamp);
        pub.writeInt((uint64_t)width);
        pub.writeInt((uint64_t)height);
        pub.writeData(data, size);
    } else {
        _queue2Frame.clear();
        _queue2Frame.push_back(zmq::message_t(
            sizeof(MsgType) 
            + sizeof(int64_t) 
            + name.length()
            + 4*sizeof(int64_t) 
            + size
        ));
        DataBuffer pub(_queue2Frame.back().data(), _queue2Frame.back().size());
        pub.writeType(MsgStreamFrame);
        pub.writeStr(name);
        pub.writeInt(timestamp);
        pub.writeInt((uint64_t)width);
        pub.writeInt((uint64_t)height);
        pub.writeData(data, size);
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
    std::list<PubValStr>& queueStream = 
        (_isWritingTo1) ? _queue2Stream : _queue1Stream;
    std::list<zmq::message_t>& queueFrame = 
        (_isWritingTo1) ? _queue2Frame : _queue1Frame;

    //Sending values Bool
    while (!queueBool.empty()) {
        //Allocate message data
        zmq::message_t packet(
            sizeof(MsgType) + sizeof(int64_t) 
            + queueBool.front().name.length()
            + sizeof(int64_t) + sizeof(uint8_t));
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
            sizeof(MsgType) + sizeof(int64_t) 
            + queueInt.front().name.length()
            + sizeof(int64_t) + sizeof(int64_t));
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
            sizeof(MsgType) + sizeof(int64_t) 
            + queueFloat.front().name.length()
            + sizeof(int64_t) + sizeof(double));
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
            sizeof(MsgType) + sizeof(int64_t) 
            + queueStr.front().name.length()
            + sizeof(int64_t) + sizeof(int64_t)
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
    //Sending values Stream
    while (!queueStream.empty()) {
        //Allocate message data
        zmq::message_t packet(
            sizeof(MsgType) + sizeof(int64_t) 
            + queueStream.front().name.length()
            + sizeof(int64_t) + sizeof(int64_t)
            + queueStream.front().value.length());
        DataBuffer pub(packet.data(), packet.size());
        pub.writeType(MsgStreamStream);
        pub.writeStr(queueStream.front().name);
        pub.writeInt(queueStream.front().timestamp);
        pub.writeStr(queueStream.front().value);

        //Send packet
        _socket.send(packet);

        //Pop value
        queueStream.pop_front();
    }
    //Sending values Frame
    while (!queueFrame.empty()) {
        //Send packet
        _socket.send(queueFrame.front());

        //Pop value
        queueFrame.pop_front();
    }
}
        
void ServerPub::swapBuffer()
{
    //Lock all publisher buffer for all types
    std::lock_guard<std::mutex> lockBool(_mutexQueueBool);
    std::lock_guard<std::mutex> lockInt(_mutexQueueInt);
    std::lock_guard<std::mutex> lockFloat(_mutexQueueFloat);
    std::lock_guard<std::mutex> lockStr(_mutexQueueStr);
    std::lock_guard<std::mutex> lockStream(_mutexQueueStream);
    std::lock_guard<std::mutex> lockFrame(_mutexQueueFrame);
    //Swap buffer
    _isWritingTo1 = !_isWritingTo1;
}

}

