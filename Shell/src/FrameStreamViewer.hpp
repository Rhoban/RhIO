#pragma once

#include <string>
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <RhIOClient.hpp>

namespace RhIO
{
/**
 * FrameStreamViewer
 *
 * Use ffplay to display real time
 * frame in raw RGB24 format
 */
class FrameStreamViewer
{
public:
  /**
   * Initialization with image size
   */
  FrameStreamViewer(const std::string& name);

  /**
   * Start and stop player instance.
   */
  void start();
  void stop();

  /**
   * Display the given image with the Player.
   * Raw image data and its size are given.
   * Image width and height size are given.
   */
  void pushFrame(const cv::Mat& frame);

private:
  /**
   * Windows name
   */
  std::string _name;

  // Window is created lazily on the streaming thread (the one that calls
  // pushFrame) because OpenCV HighGUI is not thread-safe — all GUI calls
  // must happen on the same thread.
  bool _created = false;
  bool _shouldStop = false;

public:
  bool isStopped() const
  {
    return !_created;
  }
};

}  // namespace RhIO
