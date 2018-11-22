#include <iostream>
#include <thread>
#include <chrono>
#include "RhIO.hpp"

/**
 * Test RhIO value streaming at 
 * high frequency (500Hz)
 */
int main()
{
    if (!RhIO::started()) {
        RhIO::start();
    }
    
    RhIO::Root.newInt("cycle");

    size_t cycle = 0;
    auto timeStart = std::chrono::steady_clock::now();
    while (true) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(2));
        auto timeNow = std::chrono::steady_clock::now();
        auto duration = timeNow - timeStart;
        RhIO::Root.setRTInt("cycle", cycle, 
            std::chrono::steady_clock::time_point(duration));
        cycle++;
    }
}

