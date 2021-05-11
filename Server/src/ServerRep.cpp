#include <stdexcept>
#include <iostream>
#include <list>
#include "ServerRep.hpp"
#include "Protocol.hpp"
#include "RhIO.hpp"

namespace RhIO
{
ServerRep::ServerRep(std::string endpoint) : _context(1), _socket(_context, ZMQ_REP)
{
  if (endpoint == "")
  {
    std::stringstream ss;
    ss << "tcp://*:" << (ServersPortBase + 1);
    endpoint = ss.str();
  }

  _socket.bind(endpoint.c_str());
  // Set recv timeout in ms for not
  // waiting infinitely
  int timeout = 500;
  _socket.setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
}

void ServerRep::handleRequest(bool non_blocking)
{
  auto recv_flag = (non_blocking) ? zmq::recv_flags::dontwait : zmq::recv_flags::none;
  auto send_flag = (non_blocking) ? zmq::send_flags::dontwait : zmq::send_flags::none;

  // Wait for client request
  zmq::message_t request;
  auto success = _socket.recv(request, recv_flag);

  // Return if the socket read timeout
  if (!success)
  {
    return;
  }

  // Forward all possible exception to client
  try
  {
    // Parsing it
    DataBuffer req(request.data(), request.size());
    // Check empty message
    if (req.size() == 0)
    {
      error("Empty message", send_flag);
      return;
    }
    // Select answer with message type
    MsgType type = (MsgType)req.readType();
    switch (type)
    {
      case MsgAskChildren:
        listChildren(req, send_flag);
        return;
      case MsgAskValuesBool:
        listValuesBool(req, send_flag);
        return;
      case MsgAskValuesInt:
        listValuesInt(req, send_flag);
        return;
      case MsgAskValuesFloat:
        listValuesFloat(req, send_flag);
        return;
      case MsgAskValuesStr:
        listValuesStr(req, send_flag);
        return;
      case MsgGetBool:
        getBool(req, send_flag);
        return;
      case MsgGetInt:
        getInt(req, send_flag);
        return;
      case MsgGetFloat:
        getFloat(req, send_flag);
        return;
      case MsgGetStr:
        getStr(req, send_flag);
        return;
      case MsgSetBool:
        setBool(req, send_flag);
        return;
      case MsgSetInt:
        setInt(req, send_flag);
        return;
      case MsgSetFloat:
        setFloat(req, send_flag);
        return;
      case MsgSetStr:
        setStr(req, send_flag);
        return;
      case MsgAskMetaBool:
        valMetaBool(req, send_flag);
        return;
      case MsgAskMetaInt:
        valMetaInt(req, send_flag);
        return;
      case MsgAskMetaFloat:
        valMetaFloat(req, send_flag);
        return;
      case MsgAskMetaStr:
        valMetaStr(req, send_flag);
        return;
      case MsgEnableStreamingValue:
        enableStreamingValue(req, send_flag);
        return;
      case MsgDisableStreamingValue:
        disableStreamingValue(req, send_flag);
        return;
      case MsgEnableStreamingStream:
        enableStreamingStream(req, send_flag);
        return;
      case MsgDisableStreamingStream:
        disableStreamingStream(req, send_flag);
        return;
      case MsgEnableStreamingFrame:
        enableStreamingFrame(req, send_flag);
        return;
      case MsgDisableStreamingFrame:
        disableStreamingFrame(req, send_flag);
        return;
      case MsgAskSave:
        save(req, send_flag);
        return;
      case MsgAskLoad:
        load(req, send_flag);
        return;
      case MsgAskCommands:
        listCommands(req, send_flag);
        return;
      case MsgAskAllCommands:
        listAllCommands(req, send_flag);
        return;
      case MsgAskCommandDescription:
        commandDescription(req, send_flag);
        return;
      case MsgAskCall:
        callResult(req, send_flag);
        return;
      case MsgAskStreams:
        listStreams(req, send_flag);
        return;
      case MsgAskDescriptionStream:
        descriptionStream(req, send_flag);
        return;
      case MsgAskFrames:
        listFrames(req, send_flag);
        return;
      case MsgAskMetaFrame:
        valMetaFrame(req, send_flag);
        return;
      default:
        // Unknown message type
        error("Message type not implemented", send_flag);
        return;
    }
  }
  catch (const std::logic_error& e)
  {
    error("RhIOServer logic exception: " + std::string(e.what()), send_flag);
  }
  catch (const std::runtime_error& e)
  {
    error("RhIOServer runtime exception: " + std::string(e.what()), send_flag);
  }
}

void ServerRep::listChildren(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  RhIO::IONode* node = getNode(name, send_flag);
  if (node == nullptr)
    return;

  // Compute message size
  size_t size = sizeof(MsgType);
  size += sizeof(int64_t);
  std::vector<std::string> list = node->listChildren();
  for (size_t i = 0; i < list.size(); i++)
  {
    size += sizeof(int64_t) + list[i].length();
  }

  // Allocate message data
  zmq::message_t reply(size);
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgListNames);
  rep.writeInt(list.size());
  for (size_t i = 0; i < list.size(); i++)
  {
    rep.writeStr(list[i]);
  }

  // Send reply
  _socket.send(reply, send_flag);
}

void ServerRep::listValuesBool(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  RhIO::IONode* node = getNode(name, send_flag);
  if (node == nullptr)
    return;

  // Compute message size
  size_t size = sizeof(MsgType);
  size += sizeof(int64_t);
  std::vector<std::string> list = node->listValuesBool();
  for (size_t i = 0; i < list.size(); i++)
  {
    size += sizeof(int64_t) + list[i].length();
  }

  // Allocate message data
  zmq::message_t reply(size);
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgListNames);
  rep.writeInt(list.size());
  for (size_t i = 0; i < list.size(); i++)
  {
    rep.writeStr(list[i]);
  }

  // Send reply
  _socket.send(reply, send_flag);
}
void ServerRep::listValuesInt(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  RhIO::IONode* node = getNode(name, send_flag);
  if (node == nullptr)
    return;

  // Compute message size
  size_t size = sizeof(MsgType);
  size += sizeof(int64_t);
  std::vector<std::string> list = node->listValuesInt();
  for (size_t i = 0; i < list.size(); i++)
  {
    size += sizeof(int64_t) + list[i].length();
  }

  // Allocate message data
  zmq::message_t reply(size);
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgListNames);
  rep.writeInt(list.size());
  for (size_t i = 0; i < list.size(); i++)
  {
    rep.writeStr(list[i]);
  }

  // Send reply
  _socket.send(reply, send_flag);
}
void ServerRep::listValuesFloat(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  RhIO::IONode* node = getNode(name, send_flag);
  if (node == nullptr)
    return;

  // Compute message size
  size_t size = sizeof(MsgType);
  size += sizeof(int64_t);
  std::vector<std::string> list = node->listValuesFloat();
  for (size_t i = 0; i < list.size(); i++)
  {
    size += sizeof(int64_t) + list[i].length();
  }

  // Allocate message data
  zmq::message_t reply(size);
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgListNames);
  rep.writeInt(list.size());
  for (size_t i = 0; i < list.size(); i++)
  {
    rep.writeStr(list[i]);
  }

  // Send reply
  _socket.send(reply, send_flag);
}
void ServerRep::listValuesStr(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  RhIO::IONode* node = getNode(name, send_flag);
  if (node == nullptr)
    return;

  // Compute message size
  size_t size = sizeof(MsgType);
  size += sizeof(int64_t);
  std::vector<std::string> list = node->listValuesStr();
  for (size_t i = 0; i < list.size(); i++)
  {
    size += sizeof(int64_t) + list[i].length();
  }

  // Allocate message data
  zmq::message_t reply(size);
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgListNames);
  rep.writeInt(list.size());
  for (size_t i = 0; i < list.size(); i++)
  {
    rep.writeStr(list[i]);
  }

  // Send reply
  _socket.send(reply, send_flag);
}

void ServerRep::getBool(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  // Check value name
  if (RhIO::Root.getValueType(name) != TypeBool)
  {
    error("Unknown value name: " + name, send_flag);
    return;
  }

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType) + sizeof(uint8_t));
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgValBool);
  rep.writeBool(RhIO::Root.getBool(name));

  // Send reply
  _socket.send(reply, send_flag);
}
void ServerRep::getInt(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  // Check value name
  if (RhIO::Root.getValueType(name) != TypeInt)
  {
    error("Unknown value name: " + name, send_flag);
    return;
  }

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType) + sizeof(int64_t));
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgValInt);
  rep.writeInt(RhIO::Root.getInt(name));

  // Send reply
  _socket.send(reply, send_flag);
}
void ServerRep::getFloat(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  // Check value name
  if (RhIO::Root.getValueType(name) != TypeFloat)
  {
    error("Unknown value name: " + name, send_flag);
    return;
  }

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType) + sizeof(double));
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgValFloat);
  rep.writeFloat(RhIO::Root.getFloat(name));

  // Send reply
  _socket.send(reply, send_flag);
}
void ServerRep::getStr(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  // Check value name
  if (RhIO::Root.getValueType(name) != TypeStr)
  {
    error("Unknown value name: " + name, send_flag);
    return;
  }

  // Allocate message data
  std::string str = RhIO::Root.getStr(name);
  zmq::message_t reply(sizeof(MsgType) + sizeof(int64_t) + str.length());
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgValStr);
  rep.writeStr(str);

  // Send reply
  _socket.send(reply, send_flag);
}

void ServerRep::setBool(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  // Check value name
  if (RhIO::Root.getValueType(name) != TypeBool)
  {
    error("Unknown value name: " + name, send_flag);
    return;
  }

  // Update value
  RhIO::Root.setBool(name, buffer.readBool());

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType));
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgSetOk);

  // Send reply
  _socket.send(reply, send_flag);
}
void ServerRep::setInt(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  // Check value name
  if (RhIO::Root.getValueType(name) != TypeInt)
  {
    error("Unknown value name: " + name, send_flag);
    return;
  }

  // Update value
  RhIO::Root.setInt(name, buffer.readInt());

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType));
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgSetOk);

  // Send reply
  _socket.send(reply, send_flag);
}
void ServerRep::setFloat(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  // Check value name
  if (RhIO::Root.getValueType(name) != TypeFloat)
  {
    error("Unknown value name: " + name, send_flag);
    return;
  }

  // Update value
  RhIO::Root.setFloat(name, buffer.readFloat());

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType));
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgSetOk);

  // Send reply
  _socket.send(reply, send_flag);
}
void ServerRep::setStr(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  // Check value name
  if (RhIO::Root.getValueType(name) != TypeStr)
  {
    error("Unknown value name: " + name, send_flag);
    return;
  }

  // Update value
  RhIO::Root.setStr(name, buffer.readStr());

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType));
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgSetOk);

  // Send reply
  _socket.send(reply, send_flag);
}

void ServerRep::valMetaBool(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  // Check value name
  if (RhIO::Root.getValueType(name) != TypeBool)
  {
    error("Unknown value name: " + name, send_flag);
    return;
  }

  ValueBool val = RhIO::Root.getValueBool(name);

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType) + sizeof(int64_t) + val.comment.length() + 3 * sizeof(uint8_t) +
                       sizeof(int64_t) + 3 * sizeof(uint8_t));
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

  // Send reply
  _socket.send(reply, send_flag);
}
void ServerRep::valMetaInt(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  // Check value name
  if (RhIO::Root.getValueType(name) != TypeInt)
  {
    error("Unknown value name: " + name, send_flag);
    return;
  }

  ValueInt val = RhIO::Root.getValueInt(name);

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType) + sizeof(int64_t) + val.comment.length() + 3 * sizeof(uint8_t) +
                       sizeof(int64_t) + 3 * sizeof(int64_t));
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

  // Send reply
  _socket.send(reply, send_flag);
}
void ServerRep::valMetaFloat(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  // Check value name
  if (RhIO::Root.getValueType(name) != TypeFloat)
  {
    error("Unknown value name: " + name, send_flag);
    return;
  }

  ValueFloat val = RhIO::Root.getValueFloat(name);

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType) + sizeof(int64_t) + val.comment.length() + 3 * sizeof(uint8_t) +
                       sizeof(int64_t) + 3 * sizeof(double));
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

  // Send reply
  _socket.send(reply, send_flag);
}
void ServerRep::valMetaStr(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  // Check value name
  if (RhIO::Root.getValueType(name) != TypeStr)
  {
    error("Unknown value name: " + name, send_flag);
    return;
  }

  ValueStr val = RhIO::Root.getValueStr(name);

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType) + sizeof(int64_t) + val.comment.length() + 3 * sizeof(uint8_t) +
                       sizeof(int64_t) + 3 * sizeof(int64_t) + val.min.length() + val.max.length() +
                       val.valuePersisted.length());
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

  // Send reply
  _socket.send(reply, send_flag);
}

void ServerRep::enableStreamingValue(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked value name
  std::string name = buffer.readStr();
  // Update streaming mode
  RhIO::Root.enableStreamingValue(name);

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType));
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgStreamingOK);

  // Send reply
  _socket.send(reply, send_flag);
}
void ServerRep::disableStreamingValue(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked value name
  std::string name = buffer.readStr();
  // Update streaming mode
  RhIO::Root.disableStreamingValue(name);

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType));
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgStreamingOK);

  // Send reply
  _socket.send(reply, send_flag);
}

void ServerRep::enableStreamingStream(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked stream name
  std::string name = buffer.readStr();
  // Update streaming mode
  RhIO::Root.enableStreamingStream(name);

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType));
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgStreamingOK);

  // Send reply
  _socket.send(reply, send_flag);
}
void ServerRep::disableStreamingStream(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked stream name
  std::string name = buffer.readStr();
  // Update streaming mode
  RhIO::Root.disableStreamingStream(name);

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType));
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgStreamingOK);

  // Send reply
  _socket.send(reply, send_flag);
}

void ServerRep::enableStreamingFrame(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked frame name
  std::string name = buffer.readStr();
  // Update streaming mode
  RhIO::Root.enableStreamingFrame(name);

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType));
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgStreamingOK);

  // Send reply
  _socket.send(reply, send_flag);
}
void ServerRep::disableStreamingFrame(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked frame name
  std::string name = buffer.readStr();
  // Update streaming mode
  RhIO::Root.disableStreamingFrame(name);

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType));
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgStreamingOK);

  // Send reply
  _socket.send(reply, send_flag);
}

void ServerRep::save(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  // Get path
  std::string path = buffer.readStr();

  // Get asked node
  RhIO::IONode* node = getNode(name, send_flag);
  if (node == nullptr)
    return;

  // Config save
  node->save(path);

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType));
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgPersistOK);

  // Send reply
  _socket.send(reply, send_flag);
}
void ServerRep::load(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  // Get path
  std::string path = buffer.readStr();

  // Get asked node
  RhIO::IONode* node = getNode(name, send_flag);
  if (node == nullptr)
    return;

  // Config load
  node->load(path);

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType));
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgPersistOK);

  // Send reply
  _socket.send(reply, send_flag);
}

void ServerRep::listCommands(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  RhIO::IONode* node = getNode(name, send_flag);
  if (node == nullptr)
    return;

  // Compute message size
  size_t size = sizeof(MsgType);
  size += sizeof(int64_t);
  std::vector<std::string> list = node->listCommands();
  for (size_t i = 0; i < list.size(); i++)
  {
    size += sizeof(int64_t) + list[i].length();
  }

  // Allocate message data
  zmq::message_t reply(size);
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgListNames);
  rep.writeInt(list.size());
  for (size_t i = 0; i < list.size(); i++)
  {
    rep.writeStr(list[i]);
  }

  // Send reply
  _socket.send(reply, send_flag);
}

void ServerRep::listAllCommands(DataBuffer& buffer, zmq::send_flags send_flag)
{
  (void)buffer;

  // Build all commands name
  // by iterating over the whole tree
  // All command sname set
  std::vector<std::string> list;
  // Explored nodes
  std::vector<IONode*> done;
  // Nodes to be explored
  std::list<IONode*> stack;
  // Initialization
  stack.push_back(&(RhIO::Root));
  done.push_back(&(RhIO::Root));
  // Tree traversing
  while (!stack.empty())
  {
    // Get next node to explore
    IONode* node = stack.front();
    stack.pop_front();
    // Get current node commands list
    std::vector<std::string> commands = node->listCommands();
    for (size_t i = 0; i < commands.size(); i++)
    {
      list.push_back(node->pwd() + separator + commands[i]);
    }
    // Iterate over node children
    std::vector<std::string> children = node->listChildren();
    for (size_t i = 0; i < children.size(); i++)
    {
      IONode* child = &(node->child(children[i]));
      // Check if child has benn already marked
      // as explored
      bool found = false;
      for (size_t j = 0; j < done.size(); j++)
      {
        if (done[i] == child)
        {
          found = true;
          break;
        }
      }
      if (!found)
      {
        // Add to explore queue
        stack.push_back(child);
        // Mark as explored
        done.push_back(child);
      }
    }
  }

  // Compute message size
  size_t size = sizeof(MsgType);
  size += sizeof(int64_t);
  for (size_t i = 0; i < list.size(); i++)
  {
    size += sizeof(int64_t) + list[i].length();
  }

  // Allocate message data
  zmq::message_t reply(size);
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgListNames);
  rep.writeInt(list.size());
  for (size_t i = 0; i < list.size(); i++)
  {
    rep.writeStr(list[i]);
  }

  // Send reply
  _socket.send(reply, send_flag);
}

void ServerRep::commandDescription(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked command name
  std::string name = buffer.readStr();
  // Check value name
  if (!RhIO::Root.commandExist(name))
  {
    error("Unknown command name: " + name, send_flag);
    return;
  }

  // Allocate message data
  std::string str = RhIO::Root.commandDescription(name);
  zmq::message_t reply(sizeof(MsgType) + sizeof(int64_t) + str.length());
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgCommandDescription);
  rep.writeStr(str);

  // Send reply
  _socket.send(reply, send_flag);
}
void ServerRep::callResult(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked command name
  std::string name = buffer.readStr();
  // Check value name
  if (!RhIO::Root.commandExist(name))
  {
    error("Unknown command name: " + name, send_flag);
    return;
  }

  // Build arguments list
  std::vector<std::string> list;
  size_t size = buffer.readInt();
  for (size_t i = 0; i < size; i++)
  {
    list.push_back(buffer.readStr());
  }
  // Call command
  std::string result = RhIO::Root.call(name, list);

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType) + sizeof(int64_t) + result.length());
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgCallResult);
  rep.writeStr(result);

  // Send reply
  _socket.send(reply, send_flag);
}

void ServerRep::listStreams(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  RhIO::IONode* node = getNode(name, send_flag);
  if (node == nullptr)
    return;

  // Compute message size
  size_t size = sizeof(MsgType);
  size += sizeof(int64_t);
  std::vector<std::string> list = node->listStreams();
  for (size_t i = 0; i < list.size(); i++)
  {
    size += sizeof(int64_t) + list[i].length();
  }

  // Allocate message data
  zmq::message_t reply(size);
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgListNames);
  rep.writeInt(list.size());
  for (size_t i = 0; i < list.size(); i++)
  {
    rep.writeStr(list[i]);
  }

  // Send reply
  _socket.send(reply, send_flag);
}

void ServerRep::descriptionStream(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked stream name
  std::string name = buffer.readStr();
  // Check stream name
  if (!RhIO::Root.streamExist(name))
  {
    error("Unknown stream name: " + name, send_flag);
    return;
  }

  std::string comment = RhIO::Root.streamDescription(name);

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType) + sizeof(int64_t) + comment.length() + sizeof(int64_t));
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgDescriptionStream);
  rep.writeStr(comment);

  // Send reply
  _socket.send(reply, send_flag);
}

void ServerRep::listFrames(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked node name
  std::string name = buffer.readStr();
  RhIO::IONode* node = getNode(name, send_flag);
  if (node == nullptr)
    return;

  // Compute message size
  size_t size = sizeof(MsgType);
  size += sizeof(int64_t);
  std::vector<std::string> list = node->listFrames();
  for (size_t i = 0; i < list.size(); i++)
  {
    size += sizeof(int64_t) + list[i].length();
  }

  // Allocate message data
  zmq::message_t reply(size);
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgListNames);
  rep.writeInt(list.size());
  for (size_t i = 0; i < list.size(); i++)
  {
    rep.writeStr(list[i]);
  }

  // Send reply
  _socket.send(reply, send_flag);
}

void ServerRep::valMetaFrame(DataBuffer& buffer, zmq::send_flags send_flag)
{
  // Get asked stream name
  std::string name = buffer.readStr();
  // Check stream name
  if (!RhIO::Root.frameExist(name))
  {
    error("Unknown frame name: " + name, send_flag);
    return;
  }

  Frame frame = RhIO::Root.getFrame(name);

  // Allocate message data
  zmq::message_t reply(sizeof(MsgType) + sizeof(int64_t) + frame.comment.length() + 2 * sizeof(int64_t));
  DataBuffer rep(reply.data(), reply.size());
  rep.writeType(MsgValMetaFrame);
  rep.writeStr(frame.comment);
  rep.writeInt(frame.countWatchers);

  // Send reply
  _socket.send(reply, send_flag);
}

void ServerRep::error(const std::string& msg, zmq::send_flags send_flag)
{
  // Initialize message data
  zmq::message_t reply(sizeof(MsgType) + sizeof(int64_t) + msg.length());
  DataBuffer req(reply.data(), reply.size());

  // Build error message
  req.writeType(MsgError);
  req.writeStr(msg);

  // Print error on output stream
  std::cerr << "RhIOServer error: " << msg << std::endl;

  // Send
  _socket.send(reply, send_flag);
}

RhIO::IONode* ServerRep::getNode(const std::string& name, zmq::send_flags send_flag)
{
  RhIO::IONode* node = &RhIO::Root;
  if (name != "" && name != "ROOT" && name != "/")
  {
    if (RhIO::Root.childExist(name))
    {
      node = &(RhIO::Root.child(name));
    }
    else
    {
      error("Unknown node name: " + name, send_flag);
      return nullptr;
    }
  }

  return node;
}

}  // namespace RhIO
