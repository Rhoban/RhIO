#ifndef RHIO_HPP
#define RHIO_HPP

#include "Protocol.hpp"
#include "IONode.hpp"
#include "Bind.hpp"
#include <zmq.hpp>
#include <mutex>

namespace RhIO
{
/**
 * Main RhIO global instance
 * Root of ParameterNode tree
 */
extern IONode Root;

extern std::mutex rep_mutex;

/**
 * Internal pointer to the instance of
 * the publisher server runngin in its thread
 */
class ServerPub;
extern ServerPub* ServerStream;

/**
 * Starts the RhIO server
 */
void start(unsigned int port = ServersPortBase,
           bool use_rep_mutex = false,
           bool non_blocking_rep = false);
void stop();
/**
 * Has RhIO started?
 */
bool started();

}  // namespace RhIO

#endif
