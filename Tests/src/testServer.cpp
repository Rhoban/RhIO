#include <math.h>
#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>
#include "RhIO.hpp"

int main()
{
    if (!RhIO::started) {
        RhIO::start();
    }
    assert(RhIO::started());

    RhIO::Root.newChild("test");
    RhIO::Root.newChild("test2/pouet");
    RhIO::Root.newBool("test/paramBool");
    RhIO::Root.newFloat("test/freq");
    RhIO::Root.newFloat("test/sin")
        ->minimum(-1)
        ->maximum(1);
    RhIO::Root.newInt("test/test3/paramInt")
        ->minimum(-1)
        ->maximum(10);
    RhIO::Root.child("test").newFloat("paramFloat")
        ->comment("this is a test float")
        ->defaultValue(42.0)
        ->persisted(true);
    RhIO::Root.child("test/test3").newStr("paramStr");

    RhIO::Root.newChild("server");
    RhIO::Root.child("server").newStr("hostname")
        ->defaultValue("testServ")
        ->persisted(true)
        ;

    RhIO::Root.newChild("foo");
    RhIO::Root.newChild("foo/bar");
    RhIO::Root.newFloat("foo/bar/x");
    RhIO::Root.newChild("foo/barz");
    RhIO::Root.newFloat("foo/barz/x");

    RhIO::Root.newStream("test/stream1", "Some stream");
    
    RhIO::Root.newFrame("test/frame1", "Some frame", 
        RhIO::FrameFormat::RGB);

    RhIO::Root.newCommand("test/command1",
        "command1",
        [](const std::vector<std::string>& args) -> std::string
        {
            if (args.size()!=1) {
                return "Usage: command1 arg";
            } else {
                return "OK " + args[0];
            }
        });

    std::cout << "Waiting" << std::endl;

    float angle = 0.0;
    RhIO::Bind bind("lowlevel/servos/ChevilleG");
    bind.bindNew("angle", angle);

    auto &out = RhIO::Root.out("test/stream1");

    float t = 0.0;
    while (true) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(10));
        float f = RhIO::Root.getFloat("test/freq");
        t += 0.01*f;

        RhIO::Root.setFloat("test/sin", sin(t*2*M_PI));
        out << "Debug, t=" << t << ", sin(t) = " << sin(t*2*M_PI) << std::endl << std::flush;

        angle = sin(t)*1.0;
        bind.push();
    }

    return 0;
}
