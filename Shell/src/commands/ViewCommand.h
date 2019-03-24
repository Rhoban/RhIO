#pragma once

#include <string>
#include <vector>
#include "Command.h"
#include "FrameStreamViewer.hpp"

namespace RhIO
{
class CSV;
class NodePool;
class ViewCommand : public Command
{
public:
  virtual std::string getName();
  virtual std::string getDesc();
  virtual std::string getUsage();
  virtual void process(std::vector<std::string> args);
  void update(std::string name, size_t width, size_t height, unsigned char* data, size_t size);

protected:
  std::ostream* os;

  std::vector<std::pair<std::string, FrameStreamViewer>> _viewers;
};
}  // namespace RhIO
