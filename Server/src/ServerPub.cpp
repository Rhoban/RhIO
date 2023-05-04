#include "ServerPub.hpp"
#include "Protocol.hpp"
#include "DataBuffer.hpp"

#define WATERMARK 10

namespace RhIO
{
ServerPub::ServerPub(std::string endpoint)
  : _context(1)
  , _socket(_context, ZMQ_PUB)
  , _isWritingTo1(true)
  , _queue1Bool()
  , _queue1Int()
  , _queue1Float()
  , _queue1Str()
  , _queue1Stream()
  , _queue1Frame()
  , _queue2Bool()
  , _queue2Int()
  , _queue2Float()
  , _queue2Str()
  , _queue2Stream()
  , _queue2Frame()
  , _queue1Errors()
  , _queue2Errors()
  , _mutexQueueBool()
  , _mutexQueueInt()
  , _mutexQueueFloat()
  , _mutexQueueStr()
  , _mutexQueueStream()
  , _mutexQueueFrame()
  , _mutexQueueError()
{
  if (endpoint == "")
  {
    std::stringstream ss;
    ss << "tcp://*:" << ServersPortBase;
    endpoint = ss.str();
  }

  // Limiting the water mark to 10 to avoid stacking messages or frames and
  // product a delay
  // FIXME too low warter mark value breaks Leph openGL 3d viewer
  int water_mark = WATERMARK;
  zmq_setsockopt(_socket, ZMQ_SNDHWM, &water_mark, sizeof(int));

  _socket.bind(endpoint.c_str());
}

void ServerPub::publishBool(const std::string& name, bool val, int64_t timestamp)
{
  std::lock_guard<std::mutex> lock(_mutexQueueBool);
  if (_isWritingTo1)
  {
    _queue1Bool.push_back({ name, val, timestamp });
  }
  else
  {
    _queue2Bool.push_back({ name, val, timestamp });
  }
}
void ServerPub::publishInt(const std::string& name, int64_t val, int64_t timestamp)
{
  std::lock_guard<std::mutex> lock(_mutexQueueInt);
  if (_isWritingTo1)
  {
    _queue1Int.push_back({ name, val, timestamp });
  }
  else
  {
    _queue2Int.push_back({ name, val, timestamp });
  }
}
void ServerPub::publishFloat(const std::string& name, double val, int64_t timestamp)
{
  std::lock_guard<std::mutex> lock(_mutexQueueFloat);
  if (_isWritingTo1)
  {
    _queue1Float.push_back({ name, val, timestamp });
  }
  else
  {
    _queue2Float.push_back({ name, val, timestamp });
  }
}
void ServerPub::publishStr(const std::string& name, const std::string& val, int64_t timestamp)
{
  std::lock_guard<std::mutex> lock(_mutexQueueStr);
  if (_isWritingTo1)
  {
    _queue1Str.push_back({ name, val, timestamp });
  }
  else
  {
    _queue2Str.push_back({ name, val, timestamp });
  }
}

void ServerPub::publishStream(const std::string& name, const std::string& val, int64_t timestamp)
{
  std::lock_guard<std::mutex> lock(_mutexQueueStream);
  if (_isWritingTo1)
  {
    _queue1Stream.push_back({ name, val, timestamp });
  }
  else
  {
    _queue2Stream.push_back({ name, val, timestamp });
  }
}

void ServerPub::publishFrame(const std::string& name, const cv::Mat& frame, const std::string& encoding,
                             int64_t timestamp)
{
  // Encoding image to png
  std::vector<uchar> buffer;
  cv::imencode(encoding.c_str(), frame, buffer);
  unsigned char* data = reinterpret_cast<unsigned char*>(buffer.data());
  int size = buffer.size();

  // Directly allocate message data
  std::lock_guard<std::mutex> lock(_mutexQueueFrame);
  if (_isWritingTo1)
  {
    _queue1Frame.clear();
    _queue1Frame.push_back(
        zmq::message_t(sizeof(MsgType) + sizeof(int64_t) + name.length() + 2 * sizeof(int64_t) + size));
    DataBuffer pub(_queue1Frame.back().data(), _queue1Frame.back().size());
    pub.writeType(MsgStreamFrame);
    pub.writeStr(name);
    pub.writeInt(timestamp);
    pub.writeData(data, size);
  }
  else
  {
    _queue2Frame.clear();
    _queue2Frame.push_back(
        zmq::message_t(sizeof(MsgType) + sizeof(int64_t) + name.length() + 2 * sizeof(int64_t) + size));
    DataBuffer pub(_queue2Frame.back().data(), _queue2Frame.back().size());
    pub.writeType(MsgStreamFrame);
    pub.writeStr(name);
    pub.writeInt(timestamp);
    pub.writeData(data, size);
  }
}

void ServerPub::publishError(const std::string& message)
{
  std::lock_guard<std::mutex> lock(_mutexQueueError);

  if (_isWritingTo1)
  {
    _queue1Errors.push_back(message);
  }
  else
  {
    _queue2Errors.push_back(message);
  }
}

void ServerPub::sendToClient()
{
  // Swap double buffer
  // Later network communication is lock-free
  swapBuffer();

  // Reference on full value buffer to be send
  std::list<PubValBool>& queueBool = (_isWritingTo1) ? _queue2Bool : _queue1Bool;
  std::list<PubValInt>& queueInt = (_isWritingTo1) ? _queue2Int : _queue1Int;
  std::list<PubValFloat>& queueFloat = (_isWritingTo1) ? _queue2Float : _queue1Float;
  std::list<PubValStr>& queueStr = (_isWritingTo1) ? _queue2Str : _queue1Str;
  std::list<PubValStr>& queueStream = (_isWritingTo1) ? _queue2Stream : _queue1Stream;
  std::list<zmq::message_t>& queueFrame = (_isWritingTo1) ? _queue2Frame : _queue1Frame;
  std::list<std::string>& queueErrors = (_isWritingTo1) ? _queue2Errors : _queue1Errors;
  std::set<std::string> alreadySent;

  // Sending values Bool
  while (!queueBool.empty())
  {
    if (!alreadySent.count(queueBool.front().name))
    {
      alreadySent.insert(queueBool.front().name);
      // Allocate message data
      zmq::message_t packet(sizeof(MsgType) + sizeof(int64_t) + queueBool.front().name.length() + sizeof(int64_t) +
                            sizeof(uint8_t));
      DataBuffer pub(packet.data(), packet.size());
      pub.writeType(MsgStreamBool);
      pub.writeStr(queueBool.front().name);
      pub.writeInt(queueBool.front().timestamp);
      pub.writeBool(queueBool.front().value);

      // Send packet
      _socket.send(packet);
    }
    // Pop value
    queueBool.pop_front();
  }
  // Sending values Int
  while (!queueInt.empty())
  {
    if (!alreadySent.count(queueInt.front().name))
    {
      alreadySent.insert(queueInt.front().name);
      // Allocate message data
      zmq::message_t packet(sizeof(MsgType) + sizeof(int64_t) + queueInt.front().name.length() + sizeof(int64_t) +
                            sizeof(int64_t));
      DataBuffer pub(packet.data(), packet.size());
      pub.writeType(MsgStreamInt);
      pub.writeStr(queueInt.front().name);
      pub.writeInt(queueInt.front().timestamp);
      pub.writeInt(queueInt.front().value);

      // Send packet
      _socket.send(packet);
    }

    // Pop value
    queueInt.pop_front();
  }
  // Sending values Float
  while (!queueFloat.empty())
  {
    if (!alreadySent.count(queueFloat.front().name))
    {
      alreadySent.insert(queueFloat.front().name);
      // Allocate message data
      zmq::message_t packet(sizeof(MsgType) + sizeof(int64_t) + queueFloat.front().name.length() + sizeof(int64_t) +
                            sizeof(double));
      DataBuffer pub(packet.data(), packet.size());
      pub.writeType(MsgStreamFloat);
      pub.writeStr(queueFloat.front().name);
      pub.writeInt(queueFloat.front().timestamp);
      pub.writeFloat(queueFloat.front().value);

      // Send packet
      _socket.send(packet);
    }

    // Pop value
    queueFloat.pop_front();
  }
  // Sending values Str
  while (!queueStr.empty())
  {
    if (!alreadySent.count(queueStr.front().name))
    {
      alreadySent.insert(queueStr.front().name);

      // Allocate message data
      zmq::message_t packet(sizeof(MsgType) + sizeof(int64_t) + queueStr.front().name.length() + sizeof(int64_t) +
                            sizeof(int64_t) + queueStr.front().value.length());
      DataBuffer pub(packet.data(), packet.size());
      pub.writeType(MsgStreamStr);
      pub.writeStr(queueStr.front().name);
      pub.writeInt(queueStr.front().timestamp);
      pub.writeStr(queueStr.front().value);

      // Send packet
      _socket.send(packet);
    }

    // Pop value
    queueStr.pop_front();
  }
  // Sending values Stream
  while (!queueStream.empty())
  {
    if (!alreadySent.count(queueStream.front().name))
    {
      alreadySent.insert(queueStream.front().name);
      // Allocate message data
      zmq::message_t packet(sizeof(MsgType) + sizeof(int64_t) + queueStream.front().name.length() + sizeof(int64_t) +
                            sizeof(int64_t) + queueStream.front().value.length());
      DataBuffer pub(packet.data(), packet.size());
      pub.writeType(MsgStreamStream);
      pub.writeStr(queueStream.front().name);
      pub.writeInt(queueStream.front().timestamp);
      pub.writeStr(queueStream.front().value);

      // Send packet
      _socket.send(packet);
    }

    // Pop value
    queueStream.pop_front();
  }
  // Sending values Frame
  while (!queueFrame.empty())
  {
    // Send packet
    _socket.send(queueFrame.front());

    // Pop value
    queueFrame.pop_front();
  }
  // Sending error messages
  while (!queueErrors.empty())
  {
    zmq::message_t packet(sizeof(MsgType) + sizeof(int64_t) + queueErrors.front().length());

    DataBuffer pub(packet.data(), packet.size());
    pub.writeType(MsgErrorMessage);
    pub.writeStr(queueErrors.front());

    // Send packet
    _socket.send(packet);

    queueErrors.pop_back();
  }
}

void ServerPub::swapBuffer()
{
  // Lock all publisher buffer for all types
  std::lock_guard<std::mutex> lockBool(_mutexQueueBool);
  std::lock_guard<std::mutex> lockInt(_mutexQueueInt);
  std::lock_guard<std::mutex> lockFloat(_mutexQueueFloat);
  std::lock_guard<std::mutex> lockStr(_mutexQueueStr);
  std::lock_guard<std::mutex> lockStream(_mutexQueueStream);
  std::lock_guard<std::mutex> lockFrame(_mutexQueueFrame);
  std::lock_guard<std::mutex> lockError(_mutexQueueError);
  // Swap buffer
  _isWritingTo1 = !_isWritingTo1;
}

}  // namespace RhIO
