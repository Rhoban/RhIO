#include <iomanip>
#include <sstream>
#include <iostream>
#include "Shell.h"
#include "ErrCommand.h"
#include "Node.h"
#include "NodePool.h"

namespace RhIO
{
std::string ErrCommand::getName()
{
  return "err";
}

std::string ErrCommand::getDesc()
{
  return "Lists the unread errors";
}

void ErrCommand::process(std::vector<std::string> args)
{
  Terminal::setColor("red", true);

  std::vector<std::string> errors = shell->getUnreadErrors();

  for (auto& error : errors)
  {
    std::cout << error << std::endl;
  }

  Terminal::clear();
}
}  // namespace RhIO
