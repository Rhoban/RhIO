#pragma once

#include "Command.h"

namespace RhIO
{
class CSV;
class NodePool;
class CatCommand : public Command
{
public:
  virtual std::string getName();
  virtual std::string getDesc();
  virtual std::string getUsage();
  virtual void process(std::vector<std::string> args);
  void update(std::string name, std::string message);

protected:
  std::ostream* os;
};
}  // namespace RhIO
