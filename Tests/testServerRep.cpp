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
        ->isPersisted(true);
    RhIO::Root.child("test/test3").newStr("paramStr");

    std::cout << "Waiting" << std::endl;
    std::this_thread::sleep_for(
        std::chrono::milliseconds(5000));

    return 0;
}

