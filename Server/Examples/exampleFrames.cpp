#include <unistd.h>
#include <iostream>
#include "RhIO.hpp"
#include <opencv2/opencv.hpp>

int main()
{
  RhIO::start();
  // RhIO can also be used to stream real time image
  // frames from server to client. Image are non compressed
  // in raw RGB, BGR or YUV format. Each pixels is defined by
  // 24 bits, 8 bits per channel.

  // Simple RGB 300x200 image.
  size_t width = 300;
  size_t height = 200;
  cv::Mat frame(height, width, CV_8UC3, cv::Scalar(0, 0, 0));

  // Frame have to be created before use. First argument is
  // frame relative new name. The other arguments are the
  // frame textual description,
  // and its pixel format (RGB, BGR or YUV).
  RhIO::Root.newFrame("frames/frame1", "description of frame1");

  // All frames in a node can be listed by
  // a vector of frame names.
  std::vector<std::string> list1 = RhIO::Root.child("frames").listFrames();
  for (size_t i = 0; i < list1.size(); i++)
  {
    std::cout << "Frame name: " << list1[i] << std::endl;
  }

  int xPos = 0;
  while (true)
  {
    xPos += 1;
    if (xPos >= width)
    {
      xPos = 0;
    }
    for (int k = 0; k < xPos / 13; k++)
    {
      std::cout << " ";
    }
    std::cout << "|" << std::endl;

    for (int x = 0; x < width; x++)
    {
      for (int y = 0; y < height; y++)
      {
        cv::Vec3b rgb = frame.at<cv::Vec3b>(y, x);

        rgb.val[0] = 0;
        rgb.val[1] = 0;
        rgb.val[2] = 255;

        if (x == xPos)
        {
          rgb.val[0] = 255;
        }

        frame.at<cv::Vec3b>(y, x) = rgb;
      }
    }

    cv::imwrite("/tmp/source.png", frame);

    // For performance optimization, the requested frame
    // is generated only if a client is watching it.
    if (RhIO::Root.frameIsStreaming("frames/frame1"))
    {
      // The frame data is immedialy copied from given
      // memory pointer and given size.
      //(the given size should be 3*wifth*height).
      RhIO::Root.framePush("frames/frame1", frame);
    }
    usleep(10000);
  }

  // Meta frame information can be retrieve
  // using following method.
  std::cout << "Frame description: " << RhIO::Root.getFrame("path/in/tree/frame1").comment << std::endl;
  // std::cout << "Frame format: " << (int)RhIO::Root.getFrame("path/in/tree/frame1").format << std::endl;
}
