#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>
#include "RhIO.hpp"

int main() 
{
    if (!RhIO::started()) {
        RhIO::start();
    }
    assert(RhIO::started());

    RhIO::Root.newChild("test");
    RhIO::Root.newChild("test2/pouet");
    RhIO::Root.newBool("test/paramBool");
    RhIO::Root.newInt("test/test3/paramInt")
        ->minimum(-1)
        ->maximum(10);
    RhIO::Root.child("test").newFloat("paramFloat")
        ->comment("this is a test float")
        ->defaultValue(42.0)
        ->persisted(true);
    RhIO::Root.child("test/test3").newStr("paramStr");
    
    RhIO::Root.newCommand("test/command1", 
        "command1", 
        [](const std::vector<std::string>& args) -> std::string
        {
            return "OK " + args[0];
        });
    RhIO::Root.newCommand("test/test3/command2", 
        "command2", 
        [](const std::vector<std::string>& args) -> std::string
        {
            return "OK " + args[0];
        });

    RhIO::Root.newStream("test/stream1", "stream1");
    
    RhIO::Root.newFrame("test/frame1", "frame1", 
        RhIO::FrameFormat::RGB);
    RhIO::Root.child("test").newFrame("frame2", "frame2",
        RhIO::FrameFormat::BGR);

    RhIO::start();
    assert(RhIO::started());

    std::cout << "Waiting" << std::endl;
    std::this_thread::sleep_for(
        std::chrono::milliseconds(5000));

    return 0;
}

