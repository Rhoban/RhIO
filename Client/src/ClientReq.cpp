#include <stdexcept>
#include "ClientReq.hpp"

namespace RhIO
{
ClientReq::ClientReq(const std::string& endpoint) : _context(1), _socket(_context, ZMQ_REQ)
{
  _socket.connect(endpoint.c_str());
}

std::vector<std::string> ClientReq::listChildren(const std::string& name)
{
  return listNames(MsgAskChildren, name);
}

std::vector<std::string> ClientReq::listValuesBool(const std::string& name)
{
  return listNames(MsgAskValuesBool, name);
}
std::vector<std::string> ClientReq::listValuesInt(const std::string& name)
{
  return listNames(MsgAskValuesInt, name);
}
std::vector<std::string> ClientReq::listValuesFloat(const std::string& name)
{
  return listNames(MsgAskValuesFloat, name);
}
std::vector<std::string> ClientReq::listValuesStr(const std::string& name)
{
  return listNames(MsgAskValuesStr, name);
}

std::vector<std::string> ClientReq::listCommands(const std::string& name)
{
  return listNames(MsgAskCommands, name);
}

std::vector<std::string> ClientReq::listAllCommands()
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType));
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgAskAllCommands);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  DataBuffer rep = waitReply(reply, MsgListNames);

  // Parsing reply
  size_t size = rep.readInt();
  std::vector<std::string> list;
  for (size_t k = 0; k < size; k++)
  {
    list.push_back(rep.readStr());
  }
  return list;
}

std::string ClientReq::commandDescription(const std::string& name)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgAskCommandDescription);
  req.writeStr(name);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  DataBuffer rep = waitReply(reply, MsgCommandDescription);
  return rep.readStr();
}

std::string ClientReq::call(const std::string& name, const std::vector<std::string>& arguments)
{
  // Compute data size
  size_t size = 0;
  for (size_t i = 0; i < arguments.size(); i++)
  {
    size += arguments[i].length();
  }

  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length() + sizeof(int64_t) +
                         arguments.size() * sizeof(int64_t) + size);
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgAskCall);
  req.writeStr(name);
  req.writeInt(arguments.size());
  for (size_t i = 0; i < arguments.size(); i++)
  {
    req.writeStr(arguments[i]);
  }
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  DataBuffer rep = waitReply(reply, MsgCallResult);
  return rep.readStr();
}

bool ClientReq::getBool(const std::string& name)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgGetBool);
  req.writeStr(name);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  DataBuffer rep = waitReply(reply, MsgValBool);
  return rep.readBool();
}
int64_t ClientReq::getInt(const std::string& name)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgGetInt);
  req.writeStr(name);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  DataBuffer rep = waitReply(reply, MsgValInt);
  return rep.readInt();
}
double ClientReq::getFloat(const std::string& name)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgGetFloat);
  req.writeStr(name);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  DataBuffer rep = waitReply(reply, MsgValFloat);
  return rep.readFloat();
}
std::string ClientReq::getStr(const std::string& name)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgGetStr);
  req.writeStr(name);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  DataBuffer rep = waitReply(reply, MsgValStr);
  return rep.readStr();
}

void ClientReq::setBool(const std::string& name, bool val)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length() + sizeof(uint8_t));
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgSetBool);
  req.writeStr(name);
  req.writeBool(val);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  waitReply(reply, MsgSetOk);
}
void ClientReq::setInt(const std::string& name, int64_t val)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length() + sizeof(int64_t));
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgSetInt);
  req.writeStr(name);
  req.writeInt(val);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  waitReply(reply, MsgSetOk);
}
void ClientReq::setFloat(const std::string& name, double val)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length() + sizeof(double));
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgSetFloat);
  req.writeStr(name);
  req.writeFloat(val);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  waitReply(reply, MsgSetOk);
}
void ClientReq::setStr(const std::string& name, const std::string& val)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length() + sizeof(int64_t) + val.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgSetStr);
  req.writeStr(name);
  req.writeStr(val);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  waitReply(reply, MsgSetOk);
}

ValueBool ClientReq::metaValueBool(const std::string& name)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgAskMetaBool);
  req.writeStr(name);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  DataBuffer rep = waitReply(reply, MsgValMetaBool);
  // Parse reply
  ValueBool val;
  val.name = name;
  val.comment = rep.readStr();
  val.hasMin = rep.readBool();
  val.hasMax = rep.readBool();
  val.persisted = rep.readBool();
  val.streamWatchers = rep.readInt();
  val.min = rep.readBool();
  val.max = rep.readBool();
  val.valuePersisted = rep.readBool();

  return val;
}
ValueInt ClientReq::metaValueInt(const std::string& name)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgAskMetaInt);
  req.writeStr(name);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  DataBuffer rep = waitReply(reply, MsgValMetaInt);
  // Parse reply
  ValueInt val;
  val.name = name;
  val.comment = rep.readStr();
  val.hasMin = rep.readBool();
  val.hasMax = rep.readBool();
  val.persisted = rep.readBool();
  val.streamWatchers = rep.readInt();
  val.min = rep.readInt();
  val.max = rep.readInt();
  val.valuePersisted = rep.readInt();

  return val;
}
ValueFloat ClientReq::metaValueFloat(const std::string& name)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgAskMetaFloat);
  req.writeStr(name);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  DataBuffer rep = waitReply(reply, MsgValMetaFloat);
  // Parse reply
  ValueFloat val;
  val.name = name;
  val.comment = rep.readStr();
  val.hasMin = rep.readBool();
  val.hasMax = rep.readBool();
  val.persisted = rep.readBool();
  val.streamWatchers = rep.readInt();
  val.min = rep.readFloat();
  val.max = rep.readFloat();
  val.valuePersisted = rep.readFloat();

  return val;
}
ValueStr ClientReq::metaValueStr(const std::string& name)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgAskMetaStr);
  req.writeStr(name);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  DataBuffer rep = waitReply(reply, MsgValMetaStr);
  // Parse reply
  ValueStr val;
  val.name = name;
  val.comment = rep.readStr();
  val.hasMin = rep.readBool();
  val.hasMax = rep.readBool();
  val.persisted = rep.readBool();
  val.streamWatchers = rep.readInt();
  val.min = rep.readStr();
  val.max = rep.readStr();
  val.valuePersisted = rep.readStr();

  return val;
}

void ClientReq::enableStreamingValue(const std::string& name)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgEnableStreamingValue);
  req.writeStr(name);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  waitReply(reply, MsgStreamingOK);
}
void ClientReq::disableStreamingValue(const std::string& name)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgDisableStreamingValue);
  req.writeStr(name);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  waitReply(reply, MsgStreamingOK);
}

std::vector<std::string> ClientReq::listStreams(const std::string& name)
{
  return listNames(MsgAskStreams, name);
}

std::string ClientReq::streamDescription(const std::string& name)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgAskDescriptionStream);
  req.writeStr(name);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  DataBuffer rep = waitReply(reply, MsgDescriptionStream);
  return rep.readStr();
}

std::vector<std::string> ClientReq::listFrames(const std::string& name)
{
  return listNames(MsgAskFrames, name);
}

Frame ClientReq::metaValueFrame(const std::string& name)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgAskMetaFrame);
  req.writeStr(name);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  DataBuffer rep = waitReply(reply, MsgValMetaFrame);
  // Parse reply
  Frame frame;
  frame.name = name;
  frame.comment = rep.readStr();
  frame.format = (FrameFormat)rep.readInt();
  frame.countWatchers = rep.readInt();

  return frame;
}

void ClientReq::enableStreamingStream(const std::string& name)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgEnableStreamingStream);
  req.writeStr(name);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  waitReply(reply, MsgStreamingOK);
}
void ClientReq::disableStreamingStream(const std::string& name)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgDisableStreamingStream);
  req.writeStr(name);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  waitReply(reply, MsgStreamingOK);
}

void ClientReq::enableStreamingFrame(const std::string& name)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgEnableStreamingFrame);
  req.writeStr(name);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  waitReply(reply, MsgStreamingOK);
}
void ClientReq::disableStreamingFrame(const std::string& name)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgDisableStreamingFrame);
  req.writeStr(name);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  waitReply(reply, MsgStreamingOK);
}

void ClientReq::save(const std::string& name, const std::string& serverPath)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + 2 * sizeof(int64_t) + name.length() + serverPath.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgAskSave);
  req.writeStr(name);
  req.writeStr(serverPath);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  waitReply(reply, MsgPersistOK);
}
void ClientReq::load(const std::string& name, const std::string& serverPath)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + 2 * sizeof(int64_t) + name.length() + serverPath.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(MsgAskLoad);
  req.writeStr(name);
  req.writeStr(serverPath);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  waitReply(reply, MsgPersistOK);
}

DataBuffer ClientReq::waitReply(zmq::message_t& reply, MsgType expectedType)
{
  // Wait for Server replay
  _socket.recv(&reply);
  DataBuffer rep(reply.data(), reply.size());

  // Check empty message
  if (rep.size() == 0)
  {
    error("Empty message");
  }
  // Retrieve message type
  MsgType type = (MsgType)rep.readType();
  // Check received message type
  if (type == MsgError)
  {
    error("Error message: " + rep.readStr());
  }
  else if (type != expectedType)
  {
    error("Type unexpected");
  }

  return rep;
}

void ClientReq::error(const std::string& msg)
{
  throw std::runtime_error("RhIOClient error: " + msg);
}

std::vector<std::string> ClientReq::listNames(MsgType msgType, const std::string& name)
{
  // Allocate message data
  zmq::message_t request(sizeof(MsgType) + sizeof(int64_t) + name.length());
  DataBuffer req(request.data(), request.size());
  // Build data message
  req.writeType(msgType);
  req.writeStr(name);
  // Send it
  _socket.send(request);

  // Wait for server answer
  zmq::message_t reply;
  DataBuffer rep = waitReply(reply, MsgListNames);

  // Parsing reply
  size_t size = rep.readInt();
  std::vector<std::string> list;
  for (size_t k = 0; k < size; k++)
  {
    list.push_back(rep.readStr());
  }
  return list;
}

}  // namespace RhIO
