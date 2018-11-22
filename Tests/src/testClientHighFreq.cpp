#include <iostream>
#include "RhIOClient.hpp"

/**
 * Test stream receptin at 
 * high frequency (500Hz) with
 * testServerHighFreq
 */
int main()
{
    RhIO::ClientReq clientReq(
        std::string("tcp://localhost:") + std::to_string(RhIO::PortServerRep));
    RhIO::ClientSub clientSub(
        std::string("udp://") 
        + RhIO::AddressMulticast 
        + std::string(":") 
        + std::to_string(RhIO::PortServerPub));

    clientSub.setHandlerInt(
        [](const std::string name, int64_t timestamp, int64_t val) 
    {
        std::cout << "Receiving Int: " 
            << name << " " << timestamp << " " << val << std::endl;
    });
    
    std::cout << "Waiting" << std::endl;
    clientReq.enableStreamingValue("/cycle");
    std::this_thread::sleep_for(
        std::chrono::milliseconds(5000));
    clientReq.disableStreamingValue("/cycle");

    return 0;
}

