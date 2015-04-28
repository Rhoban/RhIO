#ifndef RHIO_HPP
#define RHIO_HPP

#include "../Server/IONode.hpp"
#include "../Server/Bind.hpp"

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
 * Server replier and Server publisher port
 */
constexpr char ServerRepPort[] = "9999";
constexpr char ServerPubPort[] = "9998";

}

#endif

