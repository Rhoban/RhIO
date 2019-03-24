#include <thread>
#include <iostream>
#include "Shell.h"
#include "DelayCommand.h"
#include "RemoteCommand.h"
#include <unistd.h>

namespace RhIO
{
std::string DelayCommand::getName()
{
  return "delay";
}

std::string DelayCommand::getDesc()
{
  return "Delays a command";
}

std::string DelayCommand::getUsage()
{
  return "delay [n secs] [command]";
}

void DelayCommand::process(std::vector<std::string> args)
{
  if (args.size() < 2)
  {
    errorUsage();
  }
  else
  {
    float duration = atof(args[0].c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(1000 * duration)));

    std::string command = "";
    for (unsigned int k = 1; k < args.size(); k++)
    {
      command += args[k];
      command += " ";
    }

    shell->parse(command);
  }
}
}  // namespace RhIO
