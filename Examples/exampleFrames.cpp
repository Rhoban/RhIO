#include <iostream>
#include "RhIO.hpp"

int main()
{
    //RhIO can also be used to stream real time image
    //frames from server to client. Image are non compressed
    //in raw RGB, BGR or YUV format. Each pixels is defined by
    //24 bits, 8 bits per channel.

    //Simple RGB 300x200 image.
    size_t width = 300;
    size_t height = 200;
    RhIO::FrameFormat format = RhIO::FrameFormat::RGB;
    size_t size = 3*width*height;
    unsigned char frame[size];

    //Frame have to be created before use. First argument is
    //frame relative new name. The other arguments are the
    //frame textual description, its width and height size
    //and its pixel format (RGB, BGR or YUV).
    RhIO::Root.newFrame("path/in/tree/frame1", 
        "description of frame1", 
        width, height, format);

    //All frames in a node can be listed by
    //a vector of frame names.
    std::vector<std::string> list1 = 
        RhIO::Root.child("path/in/tree").listFrames();
    for (size_t i=0;i<list1.size();i++) {
        std::cout << "Frame name: " << list1[i] << std::endl;
    }

    //For performance optimization, the requested frame 
    //is generated only if a client is watching it.
    if (RhIO::Root.frameIsStreaming("path/in/tree/frame1")) {
        //The frame data is immedialy copied from given
        //memory pointer and given size.
        //(the given size should be 3*wifth*height).
        RhIO::Root.framePush("path/in/tree/frame1", frame, size);
    }

    //Meta frame informatin can be retrieve 
    //using following method.
    std::cout << "Frame size: " 
        << RhIO::Root.getFrame("path/in/tree/frame1").width << "x" 
        << RhIO::Root.getFrame("path/in/tree/frame1").height << std::endl;
}

