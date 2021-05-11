#ifndef RHIO_SERVERREP_HPP
#define RHIO_SERVERREP_HPP

#include <string>
#include <thread>
#include <zmq.hpp>
#include "RhIO.hpp"
#include "IONode.hpp"
#include "DataBuffer.hpp"
#include <mutex>

namespace RhIO
{
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
  ServerRep(std::string endpoint = "");

  /**
   * Wait for next Client request
   * and reply it
   */
  void handleRequest(bool non_blocking = false);

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
   * Implement MsgAskChildren reply (MsgListNames)
   */
  void listChildren(DataBuffer& buffer, zmq::send_flags send_flag);

  /**
   * Implement MsgAskValues Bool, Int, Float and Str
   * (MsgListNames)
   */
  void listValuesBool(DataBuffer& buffer, zmq::send_flags send_flag);
  void listValuesInt(DataBuffer& buffer, zmq::send_flags send_flag);
  void listValuesFloat(DataBuffer& buffer, zmq::send_flags send_flag);
  void listValuesStr(DataBuffer& buffer, zmq::send_flags send_flag);

  /**
   * Implement MsgGet for Bool, Int, Float, Str
   * (MsgVal)
   */
  void getBool(DataBuffer& buffer, zmq::send_flags send_flag);
  void getInt(DataBuffer& buffer, zmq::send_flags send_flag);
  void getFloat(DataBuffer& buffer, zmq::send_flags send_flag);
  void getStr(DataBuffer& buffer, zmq::send_flags send_flag);

  /**
   * Implement MsgSet for Bool, Int, Float, Str
   * (MsgSetOk)
   */
  void setBool(DataBuffer& buffer, zmq::send_flags send_flag);
  void setInt(DataBuffer& buffer, zmq::send_flags send_flag);
  void setFloat(DataBuffer& buffer, zmq::send_flags send_flag);
  void setStr(DataBuffer& buffer, zmq::send_flags send_flag);

  /**
   * Implement MsgAskMeta for Bool, Int, Float, Str
   * (MsgValMeta)
   */
  void valMetaBool(DataBuffer& buffer, zmq::send_flags send_flag);
  void valMetaInt(DataBuffer& buffer, zmq::send_flags send_flag);
  void valMetaFloat(DataBuffer& buffer, zmq::send_flags send_flag);
  void valMetaStr(DataBuffer& buffer, zmq::send_flags send_flag);

  /**
   * Implement MsgEnableStreamingValue and MsgDisableStreamingValue
   * (MsgStreamingOK)
   */
  void enableStreamingValue(DataBuffer& buffer, zmq::send_flags send_flag);
  void disableStreamingValue(DataBuffer& buffer, zmq::send_flags send_flag);

  /**
   * Implement MsgEnableStreamingStream and MsgDisableStreamingStream
   * (MsgStreamingOK)
   */
  void enableStreamingStream(DataBuffer& buffer, zmq::send_flags send_flag);
  void disableStreamingStream(DataBuffer& buffer, zmq::send_flags send_flag);

  /**
   * Implement MsgEnableStreamingFrame and MsgDisableStreamingFrame
   * (MsgStreamingOK)
   */
  void enableStreamingFrame(DataBuffer& buffer, zmq::send_flags send_flag);
  void disableStreamingFrame(DataBuffer& buffer, zmq::send_flags send_flag);

  /**
   * Implement MsgAskSave ans MsgAskLoad (MsgPersistOK)
   */
  void save(DataBuffer& buffer, zmq::send_flags send_flag);
  void load(DataBuffer& buffer, zmq::send_flags send_flag);

  /**
   * Implement MsgAskCommands (MsgListNames)
   */
  void listCommands(DataBuffer& buffer, zmq::send_flags send_flag);

  /**
   * Implement MsgAskAllCommands (MsgListNames)
   */
  void listAllCommands(DataBuffer& buffer, zmq::send_flags send_flag);

  /**
   * Implement MsgAskCommandDescription (MsgCommandDescription)
   */
  void commandDescription(DataBuffer& buffer, zmq::send_flags send_flag);

  /**
   * Implement MsgAskCall (MsgCallResult)
   */
  void callResult(DataBuffer& buffer, zmq::send_flags send_flag);

  /**
   * Implement MsgAskStreams (MsgListNames)
   */
  void listStreams(DataBuffer& buffer, zmq::send_flags send_flag);

  /**
   * Implement MsgAskDescriptionStream
   * (MsgDescriptionStream)
   */
  void descriptionStream(DataBuffer& buffer, zmq::send_flags send_flag);

  /**
   * Implement MsgAskFrames (MsgListNames)
   */
  void listFrames(DataBuffer& buffer, zmq::send_flags send_flag);

  /**
   * Implement MsgAskMetaFrame
   * (MsgValMetaFrame)
   */
  void valMetaFrame(DataBuffer& buffer, zmq::send_flags send_flag);

  /**
   * Implement MsgError with given error message
   */
  void error(const std::string& msg, zmq::send_flags send_flag);

  /**
   * Return the Node mapped with given absolute name.
   * If given name is invalid, nullptr is returned
   * and an error reply is send to the Client.
   */
  RhIO::IONode* getNode(const std::string& name, zmq::send_flags send_flag);
};

}  // namespace RhIO

#endif
