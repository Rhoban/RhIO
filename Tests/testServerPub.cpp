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

    std::cout << "Waiting" << std::endl;
    RhIO::Root.setBool("test/paramBool", true);
    RhIO::Root.setInt("test/test3/paramInt", 2);
    RhIO::Root.setFloat("test/paramFloat", 3.0);
    RhIO::Root.setStr("test/test3/paramStr", "4");
    RhIO::Root.setBool("test/paramBool", true);
    RhIO::Root.setInt("test/test3/paramInt", 2);
    RhIO::Root.setFloat("test/paramFloat", 3.0);
    RhIO::Root.setStr("test/test3/paramStr", "4");
    std::this_thread::sleep_for(
        std::chrono::milliseconds(100));

    RhIO::Root.setBool("test/paramBool", true);
    RhIO::Root.setInt("test/test3/paramInt", 2);
    RhIO::Root.setFloat("test/paramFloat", 3.0);
    RhIO::Root.setStr("test/test3/paramStr", "4");
    std::this_thread::sleep_for(
        std::chrono::milliseconds(100));

    RhIO::Root.setBool("test/paramBool", true);
    RhIO::Root.setInt("test/test3/paramInt", 2);
    RhIO::Root.setFloat("test/paramFloat", 3.0);
    RhIO::Root.setStr("test/test3/paramStr", "4");
    std::this_thread::sleep_for(
        std::chrono::milliseconds(100));

    RhIO::Root.setBool("test/paramBool", true);
    RhIO::Root.setInt("test/test3/paramInt", 2);
    RhIO::Root.setFloat("test/paramFloat", 3.0);
    RhIO::Root.setStr("test/test3/paramStr", "4");
    std::this_thread::sleep_for(
        std::chrono::milliseconds(1000));

    return 0;
}
