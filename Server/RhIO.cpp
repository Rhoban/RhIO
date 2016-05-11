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
std::atomic<int> initServerCount;

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

    while (true) {
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

    while (true) {
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
    std::thread* serverThreadRep = new std::thread(runServerRep);
    (void)serverThreadRep;
    std::thread* serverThreadPub = new std::thread(runServerPub);
    (void)serverThreadPub;

    //Wait until both Server are initialized
    while (initServerCount != 2) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(10));
    }
}

}

