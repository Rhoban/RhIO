#include <thread>
#include <chrono>
#include <atomic>
#include "RhIO.hpp"
#include "Server/ServerRep.hpp"
#include "Server/ServerPub.hpp"

namespace RhIO {

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

/**
 * Reply Server main loop handling
 * incomming Client request
 */
static void runServerRep()
{
    ServerRep server;
    //Notify main thread 
    //for initialization ready
    initServerCount++;

    while (!serverThreadRepOver) {
        server.handleRequest();
    }
}

/**
 * Publisher Server main loop handling
 * streaming values
 */
static void runServerPub()
{
    //Allocating ServerStream
    ServerPub server;
    ServerStream = &server;
    //Notify main thread 
    //for initialization ready
    initServerCount++;

    while (!serverThreadPubOver) {
        server.sendToClient();
        //Streaming value at 50Hz
        std::this_thread::sleep_for(
            std::chrono::milliseconds(20));
    }
}

/**
 * Create a new thread at program start
 * for Server reply and another thread for
 * Streaming Server
 * (GCC specific)
 */
static void __attribute__ ((constructor)) initThreadServer()
{ 
    //Init atomic counter
    initServerCount = 0;
    //Start Server threads
    serverThreadRepOver = false;
    serverThreadPubOver = false;
    serverThreadRep = new std::thread(runServerRep);
    serverThreadPub = new std::thread(runServerPub);

    //Wait until both Server are initialized
    while (initServerCount != 2) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(10));
    }
}

/**
 * Ask and wait Server thread ending
 */
static void __attribute__ ((destructor)) stopThreadServer()
{
    //Wait the end of server thread
    serverThreadRepOver = true;
    serverThreadPubOver = true;
    serverThreadPub->join();
    serverThreadRep->join();
    delete serverThreadPub;
    delete serverThreadRep;
}

}

