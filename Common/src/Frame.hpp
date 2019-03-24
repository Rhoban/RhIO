#ifndef RHIO_FRAME_HPP
#define RHIO_FRAME_HPP

#include <string>

namespace RhIO
{
/**
 * Frame format.
 * (3 channels, 24 bits per pixel).
 */
enum class FrameFormat : unsigned char
{
  RGB,
  BGR,
  YUV,
};

/**
 * Frame
 *
 * Structure for frame
 * image streaming.
 */
struct Frame
{
  /**
   * Frame stream name
   * and description
   */
  std::string name;
  std::string comment;

  /**
   * Frame format
   */
  FrameFormat format;

  /**
   * Frame stream current watchers count
   */
  int countWatchers;
};

}  // namespace RhIO

#endif
