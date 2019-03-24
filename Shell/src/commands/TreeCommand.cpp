#include <sstream>
#include <iostream>
#include "Shell.h"
#include "TreeCommand.h"
#include "Node.h"
#include "NodePool.h"

namespace RhIO
{
std::string TreeCommand::getName()
{
  return "tree";
}

std::string TreeCommand::getDesc()
{
  return "Lists the remote entries";
}

void TreeCommand::process(std::vector<std::string> args)
{
  auto node = getNode(args);

  showTree(node);
}

void TreeCommand::showTree(Node* node, std::string left, std::string name)
{
  Terminal::setColor("blue", true);
  std::cout << name << std::endl;

  auto all = node->getAll();
  auto children = node->getChildren();
  int k = 0, n = children.size() + all.size();
  for (auto name : children)
  {
    auto child = node->getChild(name);

    std::string subleft;
    k++;
    Terminal::setColor("white", true);
    if (k == n)
    {
      subleft = left + "    ";
      std::cout << left << "└── ";
    }
    else
    {
      subleft = left + "│   ";
      std::cout << left << "├── ";
    }
    showTree(child, subleft, name);
  }
  for (auto nodeVal : all)
  {
    k++;
    Terminal::setColor("white", true);
    if (k == n)
    {
      std::cout << left << "└── ";
    }
    else
    {
      std::cout << left << "├── ";
    }
    Terminal::setColor("white", false);
    std::cout << nodeVal.value->name << std::endl;
  }
}
}  // namespace RhIO
