#ifndef RHIO_HPP
#define RHIO_HPP

#include "Protocol.hpp"
#include "IONode.hpp"
#include "Bind.hpp"

namespace RhIO
{
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
 * Starts the RhIO server
 */
void start(unsigned int port = ServersPortBase);
void stop();
/**
 * Has RhIO started?
 */
bool started();

}  // namespace RhIO

#endif
