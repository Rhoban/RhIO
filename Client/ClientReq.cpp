#include <stdexcept>
#include "ClientReq.hpp"

namespace RhIO {

ClientReq::ClientReq(const std::string& endpoint) :
    _context(1),
    _socket(_context, ZMQ_REQ)
{
    _socket.connect(endpoint.c_str());
}
        
std::vector<std::string> ClientReq::listChildren
    (const std::string& name)
{
    return listNames(MsgAskChildren, name);
}

std::vector<std::string> ClientReq::listValuesBool
    (const std::string& name)
{
    return listNames(MsgAskValuesBool, name);
}
std::vector<std::string> ClientReq::listValuesInt
    (const std::string& name)
{
    return listNames(MsgAskValuesInt, name);
}
std::vector<std::string> ClientReq::listValuesFloat
    (const std::string& name)
{
    return listNames(MsgAskValuesFloat, name);
}
std::vector<std::string> ClientReq::listValuesStr
    (const std::string& name)
{
    return listNames(MsgAskValuesStr, name);
}

bool ClientReq::getBool(const std::string& name)
{
    //Allocate message data
    zmq::message_t request(
        sizeof(MsgType) + sizeof(long) + name.length());
    DataBuffer req(request.data(), request.size());
    //Build data message
    req.writeType(MsgGetBool);
    req.writeStr(name);
    //Send it
    _socket.send(request);

    //Wait for server answer
    zmq::message_t reply;
    DataBuffer rep = waitReply(reply, MsgValBool);
    return rep.readBool();
}
long ClientReq::getInt(const std::string& name)
{
    //Allocate message data
    zmq::message_t request(
        sizeof(MsgType) + sizeof(long) + name.length());
    DataBuffer req(request.data(), request.size());
    //Build data message
    req.writeType(MsgGetInt);
    req.writeStr(name);
    //Send it
    _socket.send(request);

    //Wait for server answer
    zmq::message_t reply;
    DataBuffer rep = waitReply(reply, MsgValInt);
    return rep.readInt();
}
double ClientReq::getFloat(const std::string& name)
{
    //Allocate message data
    zmq::message_t request(
        sizeof(MsgType) + sizeof(long) + name.length());
    DataBuffer req(request.data(), request.size());
    //Build data message
    req.writeType(MsgGetFloat);
    req.writeStr(name);
    //Send it
    _socket.send(request);

    //Wait for server answer
    zmq::message_t reply;
    DataBuffer rep = waitReply(reply, MsgValFloat);
    return rep.readFloat();
}
std::string ClientReq::getStr(const std::string& name)
{
    //Allocate message data
    zmq::message_t request(
        sizeof(MsgType) + sizeof(long) + name.length());
    DataBuffer req(request.data(), request.size());
    //Build data message
    req.writeType(MsgGetStr);
    req.writeStr(name);
    //Send it
    _socket.send(request);

    //Wait for server answer
    zmq::message_t reply;
    DataBuffer rep = waitReply(reply, MsgValStr);
    return rep.readStr();
}

void ClientReq::setBool(const std::string& name, bool val)
{
    //Allocate message data
    zmq::message_t request(
        sizeof(MsgType) + sizeof(long) 
        + name.length() + sizeof(uint8_t));
    DataBuffer req(request.data(), request.size());
    //Build data message
    req.writeType(MsgSetBool);
    req.writeStr(name);
    req.writeBool(val);
    //Send it
    _socket.send(request);

    //Wait for server answer
    zmq::message_t reply;
    waitReply(reply, MsgSetOk);
}
void ClientReq::setInt(const std::string& name, long val)
{
    //Allocate message data
    zmq::message_t request(
        sizeof(MsgType) + sizeof(long) 
        + name.length() + sizeof(long));
    DataBuffer req(request.data(), request.size());
    //Build data message
    req.writeType(MsgSetInt);
    req.writeStr(name);
    req.writeInt(val);
    //Send it
    _socket.send(request);

    //Wait for server answer
    zmq::message_t reply;
    waitReply(reply, MsgSetOk);
}
void ClientReq::setFloat(const std::string& name, double val)
{
    //Allocate message data
    zmq::message_t request(
        sizeof(MsgType) + sizeof(long) 
        + name.length() + sizeof(double));
    DataBuffer req(request.data(), request.size());
    //Build data message
    req.writeType(MsgSetFloat);
    req.writeStr(name);
    req.writeFloat(val);
    //Send it
    _socket.send(request);

    //Wait for server answer
    zmq::message_t reply;
    waitReply(reply, MsgSetOk);
}
void ClientReq::setStr(const std::string& name, 
    const std::string& val)
{
    //Allocate message data
    zmq::message_t request(
        sizeof(MsgType) + sizeof(long) 
        + name.length() + sizeof(long) + val.length());
    DataBuffer req(request.data(), request.size());
    //Build data message
    req.writeType(MsgSetStr);
    req.writeStr(name);
    req.writeStr(val);
    //Send it
    _socket.send(request);

    //Wait for server answer
    zmq::message_t reply;
    waitReply(reply, MsgSetOk);
}
        
DataBuffer ClientReq::waitReply
    (zmq::message_t& reply, MsgType expectedType)
{
    //Wait for Server replay
    _socket.recv(&reply);
    DataBuffer rep(reply.data(), reply.size());

    //Check empty message
    if (rep.size() == 0) {
        error("Empty message");
    }
    //Retrieve message type
    MsgType type = (MsgType)rep.readType();
    //Check received message type
    if (type == MsgError) {
        error("Error message: " + rep.readStr());
    } else if (type != expectedType) {
        error("Type unexpected");
    } 

    return rep;
}
        
void ClientReq::error(const std::string& msg)
{
    throw std::runtime_error("RhIOClient error: " + msg);
}
        
std::vector<std::string> ClientReq::listNames(MsgType msgType,
    const std::string& name)
{
    //Allocate message data
    zmq::message_t request(
        sizeof(MsgType) + sizeof(long) + name.length());
    DataBuffer req(request.data(), request.size());
    //Build data message
    req.writeType(msgType);
    req.writeStr(name);
    //Send it
    _socket.send(request);

    //Wait for server answer
    zmq::message_t reply;
    DataBuffer rep = waitReply(reply, MsgListNames);

    //Parsing reply
    size_t size = rep.readInt();
    std::vector<std::string> list;
    for (size_t k=0;k<size;k++) {
        list.push_back(rep.readStr());
    }
    return list;
}

}

