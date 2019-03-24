#pragma once

#include "Command.h"

namespace RhIO
{
class SyncCommand : public Command
{
public:
  virtual std::string getName();
  virtual std::string getDesc();
  virtual void process(std::vector<std::string> args);
};
}  // namespace RhIO
