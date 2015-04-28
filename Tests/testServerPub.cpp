#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>
#include "RhIO.hpp"

int main() 
{
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

    RhIO::Root.newStream("test/stream1", "stream1");

    std::cout << "Waiting" << std::endl;
    for (size_t k=0;k<50;k++) {
        RhIO::Root.setBool("test/paramBool", true);
        RhIO::Root.setInt("test/test3/paramInt", 2);
        RhIO::Root.setFloat("test/paramFloat", 3.0);
        RhIO::Root.setStr("test/test3/paramStr", "4");
        RhIO::Root.setBool("test/paramBool", true);
        RhIO::Root.setInt("test/test3/paramInt", 2);
        RhIO::Root.setFloat("test/paramFloat", 3.0);
        RhIO::Root.setStr("test/test3/paramStr", "4");
        RhIO::Root.out("test/stream1") << "test stream1" << std::endl;
        std::this_thread::sleep_for(
            std::chrono::milliseconds(100));
        if (k == 20) {
            RhIO::Root.enableStreamingValue("test/paramBool");
            RhIO::Root.enableStreamingStream("test/stream1");
        }
        if (k == 30) {
            RhIO::Root.enableStreamingValue("test/test3/paramInt");
            RhIO::Root.enableStreamingValue("test/paramFloat");
            RhIO::Root.enableStreamingValue("test/test3/paramStr");
            RhIO::Root.disableStreamingStream("test/stream1");
        }
        if (k == 40) {
            RhIO::Root.disableStreamingValue("test/paramBool");
            RhIO::Root.disableStreamingValue("test/test3/paramStr");
        }
    }

    return 0;
}
