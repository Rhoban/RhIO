#include "rhio_server/ServerPub.hpp"
#include "rhio_common/Protocol.hpp"
#include "rhio_common/DataBuffer.hpp"

namespace RhIO {

ServerPub::ServerPub(std::string endpoint) :
    _context(1),
    _socket(_context, ZMQ_PUB),
    //Maximum buffer length memory allocation
    _bufferBool(10000),
    _bufferInt(10000),
    _bufferFloat(10000),
    _bufferStr(10000),
    _bufferStream(10000),
    _isWritingTo1(true),
    _queue1Frame(),
    _queue2Frame(),
    _mutexQueueFrame()
{
    if (endpoint == "") {
        std::stringstream ss;
        ss << "tcp://*:" << PortServerPub;
        endpoint = ss.str();
    }

    // Limiting the water mark to 10 to avoid stacking messages or frames and
    // product a delay
    //int water_mark = 10;
    //zmq_setsockopt(_socket, ZMQ_SNDHWM, &water_mark, sizeof(int));

    _socket.bind(endpoint.c_str());
}

void ServerPub::publishBool(const std::string& name,
    bool val, int64_t timestamp)
{
    _bufferBool.appendFromWriter({name, val, timestamp});
}
void ServerPub::publishInt(const std::string& name,
    int64_t val, int64_t timestamp)
{
    _bufferInt.appendFromWriter({name, val, timestamp});
}
void ServerPub::publishFloat(const std::string& name,
    double val, int64_t timestamp)
{
    _bufferFloat.appendFromWriter({name, val, timestamp});
}
void ServerPub::publishStr(const std::string& name,
    const std::string& val, int64_t timestamp)
{
    _bufferStr.appendFromWriter({name, val, timestamp});
}

void ServerPub::publishStream(const std::string& name,
    const std::string& val, int64_t timestamp)
{
    _bufferStream.appendFromWriter({name, val, timestamp});
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
    const std::vector<PubValBool>& bufBool = _bufferBool.getBufferFromReader();
    size_t sizeBool = _bufferBool.getSizeFromReader();
    const std::vector<PubValInt>& bufInt = _bufferInt.getBufferFromReader();
    size_t sizeInt = _bufferInt.getSizeFromReader();
    const std::vector<PubValFloat>& bufFloat = _bufferFloat.getBufferFromReader();
    size_t sizeFloat = _bufferFloat.getSizeFromReader();
    const std::vector<PubValStr>& bufStr = _bufferStr.getBufferFromReader();
    size_t sizeStr = _bufferStr.getSizeFromReader();
    const std::vector<PubValStr>& bufStream = _bufferStream.getBufferFromReader();
    size_t sizeStream = _bufferStream.getSizeFromReader();
    std::list<zmq::message_t>& queueFrame =
        (_isWritingTo1) ? _queue2Frame : _queue1Frame;

    //Sending values Bool
    for (size_t i=0;i<sizeBool;i++) {
        //Allocate message data
        zmq::message_t packet(
            sizeof(MsgType) + sizeof(int64_t)
            + bufBool[i].name.length()
            + sizeof(int64_t) + sizeof(uint8_t));
        DataBuffer pub(packet.data(), packet.size());
        pub.writeType(MsgStreamBool);
        pub.writeStr(bufBool[i].name);
        pub.writeInt(bufBool[i].timestamp);
        pub.writeBool(bufBool[i].value);

        //Send packet
        _socket.send(packet);
    }
    //Sending values Int
    for (size_t i=0;i<sizeInt;i++) {
        //Allocate message data
        zmq::message_t packet(
            sizeof(MsgType) + sizeof(int64_t)
            + bufInt[i].name.length()
            + sizeof(int64_t) + sizeof(int64_t));
        DataBuffer pub(packet.data(), packet.size());
        pub.writeType(MsgStreamInt);
        pub.writeStr(bufInt[i].name);
        pub.writeInt(bufInt[i].timestamp);
        pub.writeInt(bufInt[i].value);

        //Send packet
        _socket.send(packet);
    }
    //Sending values Float
    for (size_t i=0;i<sizeFloat;i++) {
        //Allocate message data
        zmq::message_t packet(
            sizeof(MsgType) + sizeof(int64_t)
            + bufFloat[i].name.length()
            + sizeof(int64_t) + sizeof(double));
        DataBuffer pub(packet.data(), packet.size());
        pub.writeType(MsgStreamFloat);
        pub.writeStr(bufFloat[i].name);
        pub.writeInt(bufFloat[i].timestamp);
        pub.writeFloat(bufFloat[i].value);

        //Send packet
        _socket.send(packet);
    }
    //Sending values Str
    for (size_t i=0;i<sizeStr;i++) {
        //Allocate message data
        zmq::message_t packet(
            sizeof(MsgType) + sizeof(int64_t)
            + bufStr[i].name.length()
            + sizeof(int64_t) + sizeof(int64_t)
            + bufStr[i].value.length());
        DataBuffer pub(packet.data(), packet.size());
        pub.writeType(MsgStreamStr);
        pub.writeStr(bufStr[i].name);
        pub.writeInt(bufStr[i].timestamp);
        pub.writeStr(bufStr[i].value);

        //Send packet
        _socket.send(packet);
    }
    //Sending values Stream
    for (size_t i=0;i<sizeStream;i++) {
        //Allocate message data
        zmq::message_t packet(
            sizeof(MsgType) + sizeof(int64_t)
            + bufStream[i].name.length()
            + sizeof(int64_t) + sizeof(int64_t)
            + bufStream[i].value.length());
        DataBuffer pub(packet.data(), packet.size());
        pub.writeType(MsgStreamStream);
        pub.writeStr(bufStream[i].name);
        pub.writeInt(bufStream[i].timestamp);
        pub.writeStr(bufStream[i].value);

        //Send packet
        _socket.send(packet);
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
    _bufferBool.swapBufferFromReader();
    _bufferInt.swapBufferFromReader();
    _bufferFloat.swapBufferFromReader();
    _bufferStr.swapBufferFromReader();
    _bufferStream.swapBufferFromReader();
    std::lock_guard<std::mutex> lockFrame(_mutexQueueFrame);
    //Swap buffer
    _isWritingTo1 = !_isWritingTo1;
}

}

