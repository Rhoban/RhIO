#include <iostream>
#include <thread>
#include <cassert>
#include <chrono>
#include <thread>
#include "RhIO.hpp"

int main()
{
    const uint32_t width = 300;
    const uint32_t height = 200;
    const size_t size = 3*width*height;
    unsigned char data1[size];
    unsigned char data2[size];
    
    RhIO::Root.newFrame("test/frame1", "Some frame stream", 
        width, height, RhIO::FrameFormat::RGB);
    RhIO::Root.newFrame("test/frame2", "Some frame stream", 
        width, height, RhIO::FrameFormat::RGB);

    RhIO::Root.newInt("test/color")->defaultValue(0)->minimum(0)->maximum(255);
    RhIO::Root.newInt("test/line")->defaultValue(10)->minimum(0)->maximum(200);

    size_t line = 50;
    while (true) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(50));

        for (size_t i=0;i<size;i++) {
            data1[i] = 0;
            data2[i] = 0;
        }
        for (size_t i=0;i<height;i++) {
            for (size_t j=0;j<width;j++) {
                if (i == (size_t)RhIO::Root.getInt("test/line") || i == 10 || i == 100) {
                    data1[3*(i*width + j) + 0] = 0;
                    data1[3*(i*width + j) + 1] = 255;
                    data1[3*(i*width + j) + 2] = 0;
                } else {
                    data1[3*(i*width + j) + 0] = 0;
                    data1[3*(i*width + j) + 1] = 0;
                    data1[3*(i*width + j) + 2] = 200;
                }
                if (i == line || i == 10 || i == 100) {
                    data2[3*(i*width + j) + 0] = 255;
                    data2[3*(i*width + j) + 1] = 0;
                    data2[3*(i*width + j) + 2] = 0;
                } else {
                    data2[3*(i*width + j) + 0] = 0;
                    data2[3*(i*width + j) + 1] = RhIO::Root.getInt("test/color");
                    data2[3*(i*width + j) + 2] = 0;
                }
            }
        }
        if (RhIO::Root.frameIsStreaming("test/frame1")) {
            RhIO::Root.framePush("test/frame1", data1, size);
        }
        if (RhIO::Root.frameIsStreaming("test/frame2")) {
            RhIO::Root.framePush("test/frame2", data2, size);
        }
        line++;
        if (line > 100) {
            line = 10;
        }
    }

    return 0;
}

