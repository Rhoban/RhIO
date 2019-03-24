#include <sstream>
#include <iostream>
#include "Shell.h"
#include "LoadCommand.h"
#include <Node.h>
#include <NodePool.h>

namespace RhIO
{
std::string LoadCommand::getName()
{
  return "load";
}

std::string LoadCommand::getDesc()
{
  return "Load the values";
}

void LoadCommand::process(std::vector<std::string> args)
{
  auto node = getNode(args);
  std::string target = "rhio";
  auto name = node->getPath();
  if (name != "")
  {
    target += "/" + name;
  }

  shell->getClient()->load(node->getPath(), target);
  shell->sync();
}
}  // namespace RhIO
