#include "rhio_common/Stream.hpp"
#include "rhio_server/RhIO.hpp"
#include "rhio_server/ServerPub.hpp"

namespace RhIO {
        
StreamBuffer::StreamBuffer(const std::string& name) :
    _streamWatchers(0),
    _pwd(name)
{
}

int StreamBuffer::sync()
{
    //Publish the stream
    if (_streamWatchers > 0) {
        ServerStream->publishStream(
            _pwd, 
            str(),
            std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::steady_clock::now()
                 .time_since_epoch()).count());
    }
    //Clear buffer
    str("");
    return 0;
}

}

