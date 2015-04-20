#include <thread>
#include "RhIO.hpp"
#include "ServerRep.hpp"

#include <iostream>
#include <chrono>

namespace RhIO {

/**
 * Main instance global allocation
 */
IONode Root("ROOT", nullptr);

/**
 * Reply Server main loop handling
 * incomming Client request
 */
static void run()
{
    ServerRep server("tcp://*:9999");
    while (true) {
        server.handleRequest();
    }
}

/**
 * Create a new thread at program start
 * for Server reply
 * (GCC specific)
 */
static void __attribute__ ((constructor)) initThreadServerRep()
{ 
    std::thread* serverThread = new std::thread(run);
    (void)serverThread;
}

}

