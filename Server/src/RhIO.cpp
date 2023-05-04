#include <thread>
#include <chrono>
#include <atomic>
#include "RhIO.hpp"
#include "ServerRep.hpp"
#include "ServerPub.hpp"

namespace RhIO
{
/**
 * Main instance global allocation
 */
IONode Root("ROOT", nullptr);

/**
 * Default initialization of
 * ServerStream server
 */
ServerPub* ServerStream = nullptr;

/**
 * Atomic register storing the number
 * of initialized Server
 * (for main thread waiting)
 */
static std::atomic<int> initServerCount;

/**
 * Server Pub and Rep thread instance
 * and quit state
 */
static std::thread* serverThreadRep = nullptr;
static std::thread* serverThreadPub = nullptr;
static bool serverThreadRepOver = false;
static bool serverThreadPubOver = false;
static unsigned int port = ServersPortBase;
static bool serverStarting = false;

/**
 * Reply Server main loop handling
 * incomming Client request
 */
static void runServerRep()
{
  std::stringstream ss;
  ss << "tcp://*:" << (port + 1);
  ServerRep server(ss.str());
  // Notify main thread
  // for initialization ready
  initServerCount++;

  while (!serverThreadRepOver)
  {
    server.handleRequest();
  }
}

/**
 * Publisher Server main loop handling
 * streaming values
 */
static void runServerPub()
{
  // Allocating ServerStream
  std::stringstream ss;
  ss << "tcp://*:" << port;
  ServerPub server(ss.str());
  ServerStream = &server;
  // Notify main thread
  // for initialization ready
  initServerCount++;

  while (!serverThreadPubOver)
  {
    int64_t tsStart =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch())
            .count();
    server.sendToClient();
    int64_t tsEnd =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch())
            .count();
    int64_t duration = tsEnd - tsStart;
    // Streaming value at 50Hz
    if (duration < 10)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(10 - duration));
    }
  }
}

bool started()
{
  return serverStarting;
}

void start(unsigned int port_)
{
  serverStarting = true;
  port = port_;

  // Init atomic counter
  initServerCount = 0;
  // Start Server threads
  serverThreadRepOver = false;
  serverThreadPubOver = false;
  serverThreadRep = new std::thread(runServerRep);
  serverThreadPub = new std::thread(runServerPub);

  // Wait until both Server are initialized
  while (initServerCount != 2)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void stop()
{
  if (initServerCount > 0)
  {
    // Wait the end of server thread
    serverThreadRepOver = true;
    serverThreadPubOver = true;
    serverThreadPub->join();
    serverThreadRep->join();
    delete serverThreadPub;
    delete serverThreadRep;
  }
}

/**
 * Ask and wait Server thread ending
 */
static void __attribute__((destructor)) stopThreadServer()
{
  if (initServerCount > 0)
  {
    // Wait the end of server thread
    serverThreadRepOver = true;
    serverThreadPubOver = true;
    serverThreadPub->join();
    serverThreadRep->join();
    delete serverThreadPub;
    delete serverThreadRep;
  }
}

void errorMessage(const std::string& message)
{
  if (ServerStream != nullptr)
  {
    ServerStream->publishError(message);
  }
}

}  // namespace RhIO
