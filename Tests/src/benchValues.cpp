#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include "RhIO.hpp"

void funcSetChild()
{
    auto time1 = std::chrono::steady_clock::now();
    for (size_t i=0;i<10000;i++) {
        RhIO::Root.setInt("/path/to/child/val", 42);
    }
    auto time2 = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> dur = time2 - time1;
    std::cout << "TimeSetChild (x10000): " << dur.count() << " ms" << std::endl;
}
void funcGetChild()
{
    auto time1 = std::chrono::steady_clock::now();
    volatile int val;
    for (size_t i=0;i<10000;i++) {
        val = RhIO::Root.getInt("/path/to/child/val");
        assert(val == 42);
    }
    auto time2 = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> dur = time2 - time1;
    std::cout << "TimeGetChild (x10000): " << dur.count() << " ms" << std::endl;
}
void funcSetRoot()
{
    auto time1 = std::chrono::steady_clock::now();
    for (size_t i=0;i<10000;i++) {
        RhIO::Root.setInt("val", 42);
    }
    auto time2 = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> dur = time2 - time1;
    std::cout << "TimeSetRoot (x10000): " << dur.count() << " ms" << std::endl;
}
void funcGetRoot()
{
    auto time1 = std::chrono::steady_clock::now();
    volatile int val;
    for (size_t i=0;i<10000;i++) {
        val = RhIO::Root.getInt("val");
        assert(val == 42);
    }
    auto time2 = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> dur = time2 - time1;
    std::cout << "TimeGetRoot (x10000): " << dur.count() << " ms" << std::endl;
}
void funcSetRT()
{
    auto time1 = std::chrono::steady_clock::now();
    for (size_t i=0;i<10000;i++) {
        RhIO::Root.setRTInt("val", 42);
    }
    auto time2 = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> dur = time2 - time1;
    std::cout << "TimeSetRTRoot (x10000): " << dur.count() << " ms" << std::endl;
}
void funcGetRT()
{
    auto time1 = std::chrono::steady_clock::now();
    volatile int val;
    for (size_t i=0;i<10000;i++) {
        val = RhIO::Root.getRTInt("val");
        assert(val == 42);
    }
    auto time2 = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> dur = time2 - time1;
    std::cout << "TimeGetRTRoot (x10000): " << dur.count() << " ms" << std::endl;
}

int main()
{
    if (!RhIO::started()) {
        RhIO::start();
    }

    RhIO::Root.newChild("/path/to/child");
    RhIO::Root.newInt("/path/to/child/val");
    RhIO::Root.setInt("/path/to/child/val", 42);
    RhIO::Root.newInt("val");
    RhIO::Root.setInt("val", 42);

    std::cout << "==== with threads, with streaming" << std::endl;
    RhIO::Root.enableStreamingValue("/path/to/child/val");
    RhIO::Root.enableStreamingValue("val");
    {
        std::thread t1(funcGetChild);
        std::thread t2(funcGetChild);
        t1.join();
        t2.join();
    }
    {
        std::thread t1(funcGetRoot);
        std::thread t2(funcGetRoot);
        t1.join();
        t2.join();
    }
    {
        std::thread t1(funcGetRT);
        std::thread t2(funcGetRT);
        t1.join();
        t2.join();
    }
    {
        std::thread t1(funcSetChild);
        std::thread t2(funcSetChild);
        t1.join();
        t2.join();
    }
    {
        std::thread t1(funcSetRoot);
        std::thread t2(funcSetRoot);
        t1.join();
        t2.join();
    }
    {
        std::thread t1(funcSetRT);
        std::thread t2(funcSetRT);
        t1.join();
        t2.join();
    }
    
    std::cout << "==== with threads, no streaming" << std::endl;
    RhIO::Root.disableStreamingValue("/path/to/child/val");
    RhIO::Root.disableStreamingValue("val");
    {
        std::thread t1(funcGetChild);
        std::thread t2(funcGetChild);
        t1.join();
        t2.join();
    }
    {
        std::thread t1(funcGetRoot);
        std::thread t2(funcGetRoot);
        t1.join();
        t2.join();
    }
    {
        std::thread t1(funcGetRT);
        std::thread t2(funcGetRT);
        t1.join();
        t2.join();
    }
    {
        std::thread t1(funcSetChild);
        std::thread t2(funcSetChild);
        t1.join();
        t2.join();
    }
    {
        std::thread t1(funcSetRoot);
        std::thread t2(funcSetRoot);
        t1.join();
        t2.join();
    }
    {
        std::thread t1(funcSetRT);
        std::thread t2(funcSetRT);
        t1.join();
        t2.join();
    }

    return 0;
}

