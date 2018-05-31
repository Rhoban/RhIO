#include <sstream>
#include <math.h>
#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>
#include "RhIO.hpp"

int values = 0;

void generate(int depth=0, std::string prefix="")
{
    if (prefix != "") {
        prefix += "/";
    }

    if (depth < 4) {
        for (int k=0; k<9; k++) {
            values++;
            std::stringstream ss, ssc;
            ss << "float" << k;
            RhIO::Root.newFloat(prefix+ss.str());

            ssc << "child" << k;
            std::string name = prefix+ssc.str();
            generate(depth+1, name);
        }
    }
}

int main() 
{
    if (!RhIO::started) {
        RhIO::start();
    }
    assert(RhIO::started());

    generate();
    printf("Generated %d values\n", values);

    while (true) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(1000));
    }

    return 0;
}

