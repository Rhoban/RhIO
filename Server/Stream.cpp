#include "Stream.hpp"
#include "RhIO.hpp"
#include "ServerPub.hpp"

namespace RhIO {
        
StreamBuffer::StreamBuffer(const std::string& name) :
    _pwd(name)
{
}

int StreamBuffer::sync()
{
    //Publish the stream
    ServerStream->publishStream(_pwd, str(),
        std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::steady_clock::now()
             .time_since_epoch()).count());
    //Clear buffer
    str("");
    return 0;
}

}

