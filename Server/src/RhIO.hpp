#ifndef RHIO_HPP
#define RHIO_HPP

#include "Protocol.hpp"
#include "IONode.hpp"
#include "Bind.hpp"

namespace RhIO {

/**
 * Main RhIO global instance
 * Root of ParameterNode tree
 */
extern IONode Root;

/**
 * Internal pointer to the instance of
 * the publisher server runngin in its thread
 */
class ServerPub;
extern ServerPub* ServerStream;

/**
 * Start the RhIO server.
 *
 * @param port Network TCP port for
 * pub server and port+1 for rep server.
 * @param period Target time period in milliseconds
 * of streaming pub server loop.
 */
void start(
    unsigned int port = ServersPortBase,
    unsigned int period = 20);

/**
 * Has RhIO started?
 */
bool started();

}

#endif

