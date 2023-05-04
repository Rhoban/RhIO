#pragma once

#include <iostream>
#include "Command.h"
#include <RhIOClient.hpp>

namespace RhIO
{
class ErrCommand : public Command
{
public:
  virtual std::string getName();
  virtual std::string getDesc();
  virtual void process(std::vector<std::string> args);
};
}  // namespace RhIO
