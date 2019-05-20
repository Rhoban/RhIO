#include <iostream>
#include "Shell.h"
#include "ClearCommand.h"
#include <ncurses.h>

namespace RhIO
{
std::string ClearCommand::getName()
{
  return "clear";
}

std::string ClearCommand::getDesc()
{
  return "Erase the screen";
}

void ClearCommand::process(std::vector<std::string> args)
{
  (void)args;
  int result = system("clear");
  if (result != 0)
  {
    std::cerr << "RhIO::ClearCommand:process: Failed to clear" << std::endl;
  }
}
}  // namespace RhIO
