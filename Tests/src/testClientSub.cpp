#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>
#include "RhIO.hpp"
#include "RhIOClient.hpp"

int main() 
{
    RhIO::ClientSub client(
        std::string("udp://") 
        + RhIO::AddressMulticast 
        + std::string(":") 
        + std::to_string(RhIO::PortServerPub));
    
    client.setHandlerBool(
        [](const std::string name, int64_t timestamp, bool val) 
    {
        std::cout << "Receiving Bool:" << std::endl;
        std::cout << name << " " 
            << timestamp << " " << val << std::endl;
        assert(name == "test/paramBool");
        assert(val == true);
    });
    client.setHandlerInt(
        [](const std::string name, int64_t timestamp, int64_t val) 
    {
        std::cout << "Receiving Int:" << std::endl;
        std::cout << name << " " 
            << timestamp << " " << val << std::endl;
        assert(name == "test/test3/paramInt");
        assert(val == 2);
    });
    client.setHandlerFloat(
        [](const std::string name, int64_t timestamp, double val) 
    {
        std::cout << "Receiving Float:" << std::endl;
        std::cout << name << " " 
            << timestamp << " " << val << std::endl;
        assert(name == "test/paramFloat");
        assert(val == 3.0);
    });
    client.setHandlerStr(
        [](const std::string name, int64_t timestamp, const std::string& val) 
    {
        std::cout << "Receiving Str:" << std::endl;
        std::cout << name << " " 
            << timestamp << " " << val << std::endl;
        assert(name == "test/test3/paramStr");
        assert(val == "4");
    });
    client.setHandlerStream(
        [](const std::string name, int64_t timestamp, const std::string& val) 
    {
        std::cout << "Receiving Stream:" << std::endl;
        std::cout << name << " " 
            << timestamp << " " << val << std::endl;
        assert(name == "test/stream1");
        assert(val == "test stream1\n");
    });
    client.setHandlerFrame(
        [](const std::string name, int64_t timestamp, 
            size_t width, size_t height,
            unsigned char* data, size_t size) 
    {
        (void)data;
        std::cout << "Receiving Frame:" << std::endl;
        std::cout << name << " " << timestamp << " " 
            << width << "x" << height << " size=" << size << std::endl;
        assert(name == "test/frame1");
        assert(width == 300);
        assert(height == 200);
        assert(size == 3*300*200);
    });
    
    std::cout << "Waiting" << std::endl;
    std::this_thread::sleep_for(
        std::chrono::milliseconds(5000));

    return 0;
}

