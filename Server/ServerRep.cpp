#include <stdexcept>
#include <iostream>
#include <list>
#include "Server/ServerRep.hpp"
#include "Common/Protocol.hpp"
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
            case MsgEnableStreamingValue:
                  enableStreamingValue(req);
                  return;
            case MsgDisableStreamingValue:
                  disableStreamingValue(req);
                  return;
            case MsgEnableStreamingStream:
                  enableStreamingStream(req);
                  return;
            case MsgDisableStreamingStream:
                  disableStreamingStream(req);
                  return;
            case MsgEnableStreamingFrame:
                  enableStreamingFrame(req);
                  return;
            case MsgDisableStreamingFrame:
                  disableStreamingFrame(req);
                  return;
            case MsgAskSave:
                  save(req);
                  return;
            case MsgAskLoad:
                  load(req);
                  return;
            case MsgAskCommands:
                  listCommands(req);
                  return;
            case MsgAskAllCommands:
                  listAllCommands(req);
                  return;
            case MsgAskCommandDescription:
                  commandDescription(req);
                  return;
            case MsgAskCall:
                  callResult(req);
                  return;
            case MsgAskStreams:
                  listStreams(req);
                  return;
            case MsgAskDescriptionStream:
                  descriptionStream(req);
                  return;
            case MsgAskFrames:
                  listFrames(req);
                  return;
            case MsgAskMetaFrame:
                  valMetaFrame(req);
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
    size += sizeof(int64_t);
    std::vector<std::string> list = node->listChildren();
    for (size_t i=0;i<list.size();i++) {
        size += sizeof(int64_t) + list[i].length();
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
    size += sizeof(int64_t);
    std::vector<std::string> list = node->listValuesBool();
    for (size_t i=0;i<list.size();i++) {
        size += sizeof(int64_t) + list[i].length();
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
    size += sizeof(int64_t);
    std::vector<std::string> list = node->listValuesInt();
    for (size_t i=0;i<list.size();i++) {
        size += sizeof(int64_t) + list[i].length();
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
    size += sizeof(int64_t);
    std::vector<std::string> list = node->listValuesFloat();
    for (size_t i=0;i<list.size();i++) {
        size += sizeof(int64_t) + list[i].length();
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
    size += sizeof(int64_t);
    std::vector<std::string> list = node->listValuesStr();
    for (size_t i=0;i<list.size();i++) {
        size += sizeof(int64_t) + list[i].length();
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
    zmq::message_t reply(sizeof(MsgType) + sizeof(int64_t));
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
        sizeof(MsgType) + sizeof(int64_t) + str.length());
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
        sizeof(MsgType) + sizeof(int64_t) + val.comment.length()
        + 3*sizeof(uint8_t) + sizeof(int64_t) + 3*sizeof(uint8_t));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgValMetaBool);
    rep.writeStr(val.comment);
    rep.writeBool(val.hasMin);
    rep.writeBool(val.hasMax);
    rep.writeBool(val.persisted);
    rep.writeInt(val.streamWatchers);
    
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
        sizeof(MsgType) + sizeof(int64_t) + val.comment.length()
        + 3*sizeof(uint8_t) + sizeof(int64_t) + 3*sizeof(int64_t));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgValMetaInt);
    rep.writeStr(val.comment);
    rep.writeBool(val.hasMin);
    rep.writeBool(val.hasMax);
    rep.writeBool(val.persisted);
    rep.writeInt(val.streamWatchers);
    
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
        sizeof(MsgType) + sizeof(int64_t) + val.comment.length()
        + 3*sizeof(uint8_t) + sizeof(int64_t) + 3*sizeof(double));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgValMetaFloat);
    rep.writeStr(val.comment);
    rep.writeBool(val.hasMin);
    rep.writeBool(val.hasMax);
    rep.writeBool(val.persisted);
    rep.writeInt(val.streamWatchers);
    
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
        sizeof(MsgType) + sizeof(int64_t) + val.comment.length()
        + 3*sizeof(uint8_t) + sizeof(int64_t) + 3*sizeof(int64_t) 
        + val.min.length() + val.max.length() + val.valuePersisted.length());
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgValMetaStr);
    rep.writeStr(val.comment);
    rep.writeBool(val.hasMin);
    rep.writeBool(val.hasMax);
    rep.writeBool(val.persisted);
    rep.writeInt(val.streamWatchers);
    
    rep.writeStr(val.min);
    rep.writeStr(val.max);
    rep.writeStr(val.valuePersisted);

    //Send reply
    _socket.send(reply);
}
        
void ServerRep::enableStreamingValue(DataBuffer& buffer)
{
    //Get asked value name
    std::string name = buffer.readStr();
    //Update streaming mode
    RhIO::Root.enableStreamingValue(name);

    //Allocate message data
    zmq::message_t reply(sizeof(MsgType));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgStreamingOK);

    //Send reply
    _socket.send(reply);
}
void ServerRep::disableStreamingValue(DataBuffer& buffer)
{
    //Get asked value name
    std::string name = buffer.readStr();
    //Update streaming mode
    RhIO::Root.disableStreamingValue(name);

    //Allocate message data
    zmq::message_t reply(sizeof(MsgType));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgStreamingOK);

    //Send reply
    _socket.send(reply);
}
    
void ServerRep::enableStreamingStream(DataBuffer& buffer)
{
    //Get asked stream name
    std::string name = buffer.readStr();
    //Update streaming mode
    RhIO::Root.enableStreamingStream(name);

    //Allocate message data
    zmq::message_t reply(sizeof(MsgType));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgStreamingOK);

    //Send reply
    _socket.send(reply);
}
void ServerRep::disableStreamingStream(DataBuffer& buffer)
{
    //Get asked stream name
    std::string name = buffer.readStr();
    //Update streaming mode
    RhIO::Root.disableStreamingStream(name);

    //Allocate message data
    zmq::message_t reply(sizeof(MsgType));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgStreamingOK);

    //Send reply
    _socket.send(reply);
}

void ServerRep::enableStreamingFrame(DataBuffer& buffer)
{
    //Get asked frame name
    std::string name = buffer.readStr();
    //Update streaming mode
    RhIO::Root.enableStreamingFrame(name);

    //Allocate message data
    zmq::message_t reply(sizeof(MsgType));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgStreamingOK);

    //Send reply
    _socket.send(reply);
}
void ServerRep::disableStreamingFrame(DataBuffer& buffer)
{
    //Get asked frame name
    std::string name = buffer.readStr();
    //Update streaming mode
    RhIO::Root.disableStreamingFrame(name);

    //Allocate message data
    zmq::message_t reply(sizeof(MsgType));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgStreamingOK);

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
        
void ServerRep::listCommands(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    RhIO::IONode* node = getNode(name);
    if (node == nullptr) return;

    //Compute message size
    size_t size = sizeof(MsgType);
    size += sizeof(int64_t);
    std::vector<std::string> list = node->listCommands();
    for (size_t i=0;i<list.size();i++) {
        size += sizeof(int64_t) + list[i].length();
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

void ServerRep::listAllCommands(DataBuffer& buffer)
{
    (void) buffer;

    //Build all commands name 
    //by iterating over the whole tree
    //All command sname set
    std::vector<std::string> list;
    //Explored nodes
    std::vector<IONode*> done;
    //Nodes to be explored
    std::list<IONode*> stack;
    //Initialization
    stack.push_back(&(RhIO::Root));
    done.push_back(&(RhIO::Root));
    //Tree traversing
    while (!stack.empty()) {
        //Get next node to explore
        IONode* node = stack.front();
        stack.pop_front();
        //Get current node commands list
        std::vector<std::string> commands = node->listCommands();
        for (size_t i=0;i<commands.size();i++) {
            list.push_back(node->pwd() + separator + commands[i]);
        }
        //Iterate over node children
        std::vector<std::string> children = node->listChildren();
        for (size_t i=0;i<children.size();i++) {
            IONode* child = &(node->child(children[i]));
            //Check if child has benn already marked
            //as explored
            bool found = false;
            for (size_t j=0;j<done.size();j++) {
                if (done[i] == child) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                //Add to explore queue
                stack.push_back(child);
                //Mark as explored
                done.push_back(child);
            }
        }
    }

    //Compute message size
    size_t size = sizeof(MsgType);
    size += sizeof(int64_t);
    for (size_t i=0;i<list.size();i++) {
        size += sizeof(int64_t) + list[i].length();
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

void ServerRep::commandDescription(DataBuffer& buffer)
{
    //Get asked command name
    std::string name = buffer.readStr();
    //Check value name
    if (!RhIO::Root.commandExist(name)) {
        error("Unknown command name: " + name);
        return;
    }

    //Allocate message data
    std::string str = RhIO::Root.commandDescription(name);
    zmq::message_t reply(
        sizeof(MsgType) + sizeof(int64_t) + str.length());
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgCommandDescription);
    rep.writeStr(str);

    //Send reply
    _socket.send(reply);
}
void ServerRep::callResult(DataBuffer& buffer)
{
    //Get asked command name
    std::string name = buffer.readStr();
    //Check value name
    if (!RhIO::Root.commandExist(name)) {
        error("Unknown command name: " + name);
        return;
    }

    //Build arguments list
    std::vector<std::string> list;
    size_t size = buffer.readInt();
    for (size_t i=0;i<size;i++) {
        list.push_back(buffer.readStr());
    }
    //Call command
    std::string result = RhIO::Root.call(name, list);

    //Allocate message data
    zmq::message_t reply(
        sizeof(MsgType) + sizeof(int64_t) + result.length());
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgCallResult);
    rep.writeStr(result);

    //Send reply
    _socket.send(reply);
}
        
void ServerRep::listStreams(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    RhIO::IONode* node = getNode(name);
    if (node == nullptr) return;

    //Compute message size
    size_t size = sizeof(MsgType);
    size += sizeof(int64_t);
    std::vector<std::string> list = node->listStreams();
    for (size_t i=0;i<list.size();i++) {
        size += sizeof(int64_t) + list[i].length();
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
        
void ServerRep::descriptionStream(DataBuffer& buffer)
{
    //Get asked stream name
    std::string name = buffer.readStr();
    //Check stream name
    if (!RhIO::Root.streamExist(name)) {
        error("Unknown stream name: " + name);
        return;
    }

    std::string comment = RhIO::Root.streamDescription(name);

    //Allocate message data
    zmq::message_t reply(
        sizeof(MsgType) + sizeof(int64_t) + comment.length()
        + sizeof(int64_t));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgDescriptionStream);
    rep.writeStr(comment);

    //Send reply
    _socket.send(reply);
}
        
void ServerRep::listFrames(DataBuffer& buffer)
{
    //Get asked node name
    std::string name = buffer.readStr();
    RhIO::IONode* node = getNode(name);
    if (node == nullptr) return;

    //Compute message size
    size_t size = sizeof(MsgType);
    size += sizeof(int64_t);
    std::vector<std::string> list = node->listFrames();
    for (size_t i=0;i<list.size();i++) {
        size += sizeof(int64_t) + list[i].length();
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

void ServerRep::valMetaFrame(DataBuffer& buffer)
{
    //Get asked stream name
    std::string name = buffer.readStr();
    //Check stream name
    if (!RhIO::Root.frameExist(name)) {
        error("Unknown frame name: " + name);
        return;
    }

    Frame frame = RhIO::Root.getFrame(name);

    //Allocate message data
    zmq::message_t reply(
        sizeof(MsgType) + sizeof(int64_t) + frame.comment.length()
        + 2*sizeof(int64_t));
    DataBuffer rep(reply.data(), reply.size());
    rep.writeType(MsgValMetaFrame);
    rep.writeStr(frame.comment);
    rep.writeInt((uint64_t)frame.format);
    rep.writeInt(frame.countWatchers);
    
    //Send reply
    _socket.send(reply);
}
        
void ServerRep::error(const std::string& msg)
{
    //Initialize message data
    zmq::message_t reply(
        sizeof(MsgType) + sizeof(int64_t) + msg.length());
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

