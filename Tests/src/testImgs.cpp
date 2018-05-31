#include <iostream>
#include <thread>
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

    RhIO::Root.newFrame("test/frame1", "Some frame stream", 
        RhIO::FrameFormat::RGB);
    RhIO::IONode& node = RhIO::Root.child("test"); 
    node.newFrame("frame2", "Some frame stream", 
        RhIO::FrameFormat::RGB);

    RhIO::Root.newInt("test/color")->defaultValue(0)->minimum(0)->maximum(255);
    RhIO::Root.newInt("test/line")->defaultValue(10)->minimum(0)->maximum(200);
    RhIO::Root.newInt("test/width")->defaultValue(300)->minimum(1)->maximum(800);

    size_t line = 50;
    while (true) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(50));

        const uint32_t width1 = 300;
        const uint32_t height1 = 200;
        const size_t size1 = 3*width1*height1;
        const uint32_t width2 = RhIO::Root.getInt("test/width");
        const uint32_t height2 = 200;
        const size_t size2 = 3*width2*height2;
        unsigned char data1[size1];
        unsigned char data2[size2];

        for (size_t i=0;i<size1;i++) {
            data1[i] = 0;
        }
        for (size_t i=0;i<size2;i++) {
            data2[i] = 0;
        }
        for (size_t i=0;i<height1;i++) {
            for (size_t j=0;j<width1;j++) {
                if (i == (size_t)RhIO::Root.getInt("test/line") || i == 10 || i == 100) {
                    data1[3*(i*width1 + j) + 0] = 0;
                    data1[3*(i*width1 + j) + 1] = 255;
                    data1[3*(i*width1 + j) + 2] = 0;
                } else {
                    data1[3*(i*width1 + j) + 0] = 0;
                    data1[3*(i*width1 + j) + 1] = 0;
                    data1[3*(i*width1 + j) + 2] = 200;
                }
            }
        }
        for (size_t i=0;i<height2;i++) {
            for (size_t j=0;j<width2;j++) {
                if (i == line || i == 10 || i == 100) {
                    data2[3*(i*width2 + j) + 0] = 255;
                    data2[3*(i*width2 + j) + 1] = 0;
                    data2[3*(i*width2 + j) + 2] = 0;
                } else {
                    data2[3*(i*width2 + j) + 0] = 0;
                    data2[3*(i*width2 + j) + 1] = RhIO::Root.getInt("test/color");
                    data2[3*(i*width2 + j) + 2] = 0;
                }
            }
        }
        if (RhIO::Root.frameIsStreaming("test/frame1")) {
            RhIO::Root.framePush("test/frame1", 
                width1, height1, data1, size1);
        }
        if (RhIO::Root.frameIsStreaming("test/frame2")) {
            RhIO::Root.framePush("test/frame2", 
                width2, height2, data2, size2);
        }
        line++;
        if (line > 100) {
            line = 10;
        }
    }

    return 0;
}

