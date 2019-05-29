#include "FrameStreamViewer.hpp"

namespace RhIO
{
FrameStreamViewer::FrameStreamViewer(const std::string& name) : _name(name)
{
}

void FrameStreamViewer::start()
{
  cv::namedWindow(_name, cv::WINDOW_NORMAL);
}

void FrameStreamViewer::stop()
{
  cv::destroyWindow(_name);
}

void FrameStreamViewer::pushFrame(const cv::Mat& frame)
{
  cv::imshow(_name, frame);
  cv::imwrite("/tmp/out.png", frame);
  cv::waitKey(1);
}
}  // namespace RhIO
