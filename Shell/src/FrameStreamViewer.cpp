#include "FrameStreamViewer.hpp"

namespace RhIO
{
FrameStreamViewer::FrameStreamViewer(const std::string& name) : _name(name)
{
}

void FrameStreamViewer::start()
{
}

void FrameStreamViewer::stop()
{
  // Window is destroyed by pushFrame on the streaming thread (HighGUI is not
  // thread-safe — all GUI calls must happen on the same thread that created
  // the window). The caller must keep the frame callback alive long enough
  // for one more pushFrame to fire.
  _shouldStop = true;
}

void FrameStreamViewer::pushFrame(const cv::Mat& frame)
{
  if (_shouldStop)
  {
    if (_created)
    {
      cv::destroyWindow(_name);
      cv::waitKey(1);
      _created = false;
    }
    return;
  }
  if (!_created)
  {
    cv::namedWindow(_name, cv::WINDOW_NORMAL);
    _created = true;
  }
  cv::resizeWindow(_name, frame.cols, frame.rows);
  cv::imshow(_name, frame);
  cv::waitKey(1);
}
}  // namespace RhIO
