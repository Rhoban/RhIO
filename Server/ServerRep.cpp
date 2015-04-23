#include <stdexcept>
#include <iostream>
#include "ServerRep.hpp"
#include "Protocol.hpp"
#include "RhIO.hpp"

namespace RhIO {

ServerRep::ServerRep(const std::string& endpoint) :
    _context(1),
    _socket(_context, ZMQ_REP)
{
    _socket.bind(endpoint.c_str());
}
        
void ServerRep::handleRequest()
{
    //Wait for client request
    zmq::message_t request;
    _socket.recv(&request);

    //Forward all possible exception to client
    try {
        //Parsing it
        DataBuffer req(request.data(), request.size());
        //Check empty message
        if (req.size() == 0) {
            error("Empty message");
            return;
        }
        //Select answer with message type
        MsgType type = (MsgType)req.readType();
        switch (type) {
            case MsgAskChildren:
                listChildren(req);
                return;
            case MsgAskValuesBool:
                listValuesBool(req);
                return;
            case MsgAskValuesInt:
                listValuesInt(req);
                return;
            case MsgAskValuesFloat:
                listValuesFloat(req);
                return;
            case MsgAskValuesStr:
                listValuesStr(req);
                return;
            case MsgGetBool:
                getBool(req);
                return;
            case MsgGetInt:
                getInt(req);
                return;
            case MsgGetFloat:
                getFloat(req);
                return;
            case MsgGetStr:
                getStr(req);
                return;
            case MsgSetBool:
                  setBool(req);
                  return;
            case MsgSetInt:
                  setInt(req);
                  return;
            case MsgSetFloat:
                  setFloat(req);
                  return;
            case MsgSetStr:
                  setStr(req);
                  return;
            case MsgAskMetaBool:
                  valMetaBool(req);
                  return;
            case MsgAskMetaInt:
                  valMetaInt(req);
                  return;
            case MsgAskMetaFloat:
                  valMetaFloat(req);
                  return;
            case MsgAskMetaStr:
                  valMetaStr(req);
                  return;
            case MsgAskSave:
                  save(req);
                  return;
            case MsgAskLoad:
                  load(req);
                  return;
            default:
                //Unknown message type
                error("Message type not implemented");
                return;
        }
    } catch (const std::logic_error& e) {
        error("RhIOServer logic exception: " + std::string(e.what()));
    } catch (const std::runtime_error& e) {
        error("RhIOServer runtime exception: " + std::string(e.what()));
    }
}
        
void ServerRep::listChildren(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    RhIO::IONode* node = getNode(name);
    if (node == nullptr) return;

    //Compute message size
    size_t size = sizeof(MsgType);
    size += sizeof(long);
    std::vector<std::string> list = node->listChildren();
    for (size_t i=0;i<list.size();i++) {
        size += sizeof(long) + list[i].length();
    }

    //Allocate message data
    zmq::message_t reply(size);
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgListNames);
    rep.writeInt(list.size());
    for (size_t i=0;i<list.size();i++) {
        rep.writeStr(list[i]);
    }

    //Send reply
    _socket.send(reply);
}
        
void ServerRep::listValuesBool(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    RhIO::IONode* node = getNode(name);
    if (node == nullptr) return;

    //Compute message size
    size_t size = sizeof(MsgType);
    size += sizeof(long);
    std::vector<std::string> list = node->listValuesBool();
    for (size_t i=0;i<list.size();i++) {
        size += sizeof(long) + list[i].length();
    }

    //Allocate message data
    zmq::message_t reply(size);
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgListNames);
    rep.writeInt(list.size());
    for (size_t i=0;i<list.size();i++) {
        rep.writeStr(list[i]);
    }

    //Send reply
    _socket.send(reply);
}
void ServerRep::listValuesInt(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    RhIO::IONode* node = getNode(name);
    if (node == nullptr) return;

    //Compute message size
    size_t size = sizeof(MsgType);
    size += sizeof(long);
    std::vector<std::string> list = node->listValuesInt();
    for (size_t i=0;i<list.size();i++) {
        size += sizeof(long) + list[i].length();
    }

    //Allocate message data
    zmq::message_t reply(size);
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgListNames);
    rep.writeInt(list.size());
    for (size_t i=0;i<list.size();i++) {
        rep.writeStr(list[i]);
    }

    //Send reply
    _socket.send(reply);
}
void ServerRep::listValuesFloat(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    RhIO::IONode* node = getNode(name);
    if (node == nullptr) return;

    //Compute message size
    size_t size = sizeof(MsgType);
    size += sizeof(long);
    std::vector<std::string> list = node->listValuesFloat();
    for (size_t i=0;i<list.size();i++) {
        size += sizeof(long) + list[i].length();
    }

    //Allocate message data
    zmq::message_t reply(size);
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgListNames);
    rep.writeInt(list.size());
    for (size_t i=0;i<list.size();i++) {
        rep.writeStr(list[i]);
    }

    //Send reply
    _socket.send(reply);
}
void ServerRep::listValuesStr(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    RhIO::IONode* node = getNode(name);
    if (node == nullptr) return;

    //Compute message size
    size_t size = sizeof(MsgType);
    size += sizeof(long);
    std::vector<std::string> list = node->listValuesStr();
    for (size_t i=0;i<list.size();i++) {
        size += sizeof(long) + list[i].length();
    }

    //Allocate message data
    zmq::message_t reply(size);
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgListNames);
    rep.writeInt(list.size());
    for (size_t i=0;i<list.size();i++) {
        rep.writeStr(list[i]);
    }

    //Send reply
    _socket.send(reply);
}

void ServerRep::getBool(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    //Check value name
    if (RhIO::Root.getValueType(name) != TypeBool) {
        error("Unknown value name: " + name);
        return;
    }

    //Allocate message data
    zmq::message_t reply(sizeof(MsgType) + sizeof(uint8_t));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgValBool);
    rep.writeBool(RhIO::Root.getBool(name));

    //Send reply
    _socket.send(reply);
}
void ServerRep::getInt(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    //Check value name
    if (RhIO::Root.getValueType(name) != TypeInt) {
        error("Unknown value name: " + name);
        return;
    }

    //Allocate message data
    zmq::message_t reply(sizeof(MsgType) + sizeof(long));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgValInt);
    rep.writeInt(RhIO::Root.getInt(name));

    //Send reply
    _socket.send(reply);
}
void ServerRep::getFloat(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    //Check value name
    if (RhIO::Root.getValueType(name) != TypeFloat) {
        error("Unknown value name: " + name);
        return;
    }

    //Allocate message data
    zmq::message_t reply(sizeof(MsgType) + sizeof(double));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgValFloat);
    rep.writeFloat(RhIO::Root.getFloat(name));

    //Send reply
    _socket.send(reply);
}
void ServerRep::getStr(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    //Check value name
    if (RhIO::Root.getValueType(name) != TypeStr) {
        error("Unknown value name: " + name);
        return;
    }

    //Allocate message data
    std::string str = RhIO::Root.getStr(name);
    zmq::message_t reply(
        sizeof(MsgType) + sizeof(long) + str.length());
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgValStr);
    rep.writeStr(str);

    //Send reply
    _socket.send(reply);
}

void ServerRep::setBool(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    //Check value name
    if (RhIO::Root.getValueType(name) != TypeBool) {
        error("Unknown value name: " + name);
        return;
    }

    //Update value
    RhIO::Root.setBool(name, buffer.readBool());

    //Allocate message data
    zmq::message_t reply(sizeof(MsgType));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgSetOk);

    //Send reply
    _socket.send(reply);
}
void ServerRep::setInt(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    //Check value name
    if (RhIO::Root.getValueType(name) != TypeInt) {
        error("Unknown value name: " + name);
        return;
    }

    //Update value
    RhIO::Root.setInt(name, buffer.readInt());

    //Allocate message data
    zmq::message_t reply(sizeof(MsgType));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgSetOk);

    //Send reply
    _socket.send(reply);
}
void ServerRep::setFloat(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    //Check value name
    if (RhIO::Root.getValueType(name) != TypeFloat) {
        error("Unknown value name: " + name);
        return;
    }

    //Update value
    RhIO::Root.setFloat(name, buffer.readFloat());

    //Allocate message data
    zmq::message_t reply(sizeof(MsgType));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgSetOk);

    //Send reply
    _socket.send(reply);
}
void ServerRep::setStr(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    //Check value name
    if (RhIO::Root.getValueType(name) != TypeStr) {
        error("Unknown value name: " + name);
        return;
    }

    //Update value
    RhIO::Root.setStr(name, buffer.readStr());

    //Allocate message data
    zmq::message_t reply(sizeof(MsgType));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgSetOk);

    //Send reply
    _socket.send(reply);
}

void ServerRep::valMetaBool(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    //Check value name
    if (RhIO::Root.getValueType(name) != TypeBool) {
        error("Unknown value name: " + name);
        return;
    }

    ValueBool val = RhIO::Root.getValueBool(name);

    //Allocate message data
    zmq::message_t reply(
        sizeof(MsgType) + sizeof(long) + val.comment.length()
        + 3*sizeof(uint8_t) + 3*sizeof(uint8_t));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgValMetaBool);
    rep.writeStr(val.comment);
    rep.writeBool(val.hasMin);
    rep.writeBool(val.hasMax);
    rep.writeBool(val.persisted);
    
    rep.writeBool(val.min);
    rep.writeBool(val.max);
    rep.writeBool(val.valuePersisted);

    //Send reply
    _socket.send(reply);
}
void ServerRep::valMetaInt(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    //Check value name
    if (RhIO::Root.getValueType(name) != TypeInt) {
        error("Unknown value name: " + name);
        return;
    }

    ValueInt val = RhIO::Root.getValueInt(name);

    //Allocate message data
    zmq::message_t reply(
        sizeof(MsgType) + sizeof(long) + val.comment.length()
        + 3*sizeof(uint8_t) + 3*sizeof(long));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgValMetaInt);
    rep.writeStr(val.comment);
    rep.writeBool(val.hasMin);
    rep.writeBool(val.hasMax);
    rep.writeBool(val.persisted);
    
    rep.writeInt(val.min);
    rep.writeInt(val.max);
    rep.writeInt(val.valuePersisted);

    //Send reply
    _socket.send(reply);
}
void ServerRep::valMetaFloat(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    //Check value name
    if (RhIO::Root.getValueType(name) != TypeFloat) {
        error("Unknown value name: " + name);
        return;
    }

    ValueFloat val = RhIO::Root.getValueFloat(name);

    //Allocate message data
    zmq::message_t reply(
        sizeof(MsgType) + sizeof(long) + val.comment.length()
        + 3*sizeof(uint8_t) + 3*sizeof(double));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgValMetaFloat);
    rep.writeStr(val.comment);
    rep.writeBool(val.hasMin);
    rep.writeBool(val.hasMax);
    rep.writeBool(val.persisted);
    
    rep.writeFloat(val.min);
    rep.writeFloat(val.max);
    rep.writeFloat(val.valuePersisted);

    //Send reply
    _socket.send(reply);
}
void ServerRep::valMetaStr(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    //Check value name
    if (RhIO::Root.getValueType(name) != TypeStr) {
        error("Unknown value name: " + name);
        return;
    }

    ValueStr val = RhIO::Root.getValueStr(name);

    //Allocate message data
    zmq::message_t reply(
        sizeof(MsgType) + sizeof(long) + val.comment.length()
        + 3*sizeof(uint8_t) + 3*sizeof(long) 
        + val.min.length() + val.max.length() + val.valuePersisted.length());
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgValMetaStr);
    rep.writeStr(val.comment);
    rep.writeBool(val.hasMin);
    rep.writeBool(val.hasMax);
    rep.writeBool(val.persisted);
    
    rep.writeStr(val.min);
    rep.writeStr(val.max);
    rep.writeStr(val.valuePersisted);

    //Send reply
    _socket.send(reply);
}
    
void ServerRep::save(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    //Get path
    std::string path = buffer.readStr();
    
    //Get asked node
    RhIO::IONode* node = getNode(name);
    if (node == nullptr) return;

    //Config save
    node->save(path);
    
    //Allocate message data
    zmq::message_t reply(sizeof(MsgType));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgPersistOK);

    //Send reply
    _socket.send(reply);
}
void ServerRep::load(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    //Get path
    std::string path = buffer.readStr();
    
    //Get asked node
    RhIO::IONode* node = getNode(name);
    if (node == nullptr) return;

    //Config load
    node->load(path);
    
    //Allocate message data
    zmq::message_t reply(sizeof(MsgType));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgPersistOK);

    //Send reply
    _socket.send(reply);
}
        
void ServerRep::error(const std::string& msg)
{
    //Initialize message data
    zmq::message_t reply(
        sizeof(MsgType) + sizeof(long) + msg.length());
    DataBuffer req(reply.data(), reply.size());
    
    //Build error message
    req.writeType(MsgError);
    req.writeStr(msg);

    //Print error on output stream
    std::cerr << "RhIOServer error: " << msg << std::endl;

    //Send
    _socket.send(reply);
}
        
RhIO::IONode* ServerRep::getNode(const std::string& name)
{
    RhIO::IONode* node = &RhIO::Root;
    if (name != "" && name != "ROOT" && name != "/") {
        if (RhIO::Root.childExist(name)) {
            node = &(RhIO::Root.child(name));
        } else {
            error("Unknown node name: " + name);
            return nullptr;
        }
    }

    return node;
}

}

