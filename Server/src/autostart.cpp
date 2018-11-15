#include "RhIO.hpp"

namespace RhIO
{

/**
 * Create a new thread at program start
 * for Server reply and another thread for
 * Streaming Server
 * (GCC specific)
 */
static void __attribute__ ((constructor)) initThreadServer()
{ 
    start();
}

}
