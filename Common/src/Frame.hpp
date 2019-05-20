#ifndef RHIO_FRAME_HPP
#define RHIO_FRAME_HPP

#include <string>

namespace RhIO
{
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
   * Frame stream current watchers count
   */
  int countWatchers;
};

}  // namespace RhIO

#endif
