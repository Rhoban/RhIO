#include <stdexcept>
#include <sstream>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "Shell.h"
#include "Command.h"
#include "Terminal.h"

namespace RhIO
{
Command::~Command()
{
}

void Command::setShell(Shell* shell_)
{
  shell = shell_;
}

std::string Command::getUsage()
{
  return "";
}

void Command::errorUsage()
{
  std::stringstream ss;
  ss << "Usage: " << getUsage();
  throw std::runtime_error(ss.str());
}

Node* Command::getNode(std::vector<std::string> args)
{
  std::string dir = "";

  if (args.size())
  {
    dir = args[0];
  }

  Node* node = shell->getNode(dir);
  if (node == NULL)
  {
    std::stringstream ss;
    ss << "Unable to get node " << dir;
    throw std::runtime_error(ss.str());
  }

  return node;
}

std::ostream* Command::getStream(std::vector<std::string>& args)
{
  int n = args.size();
  if (n >= 2)
  {
    if (args[n - 2] == ">")
    {
      auto name = args.back();
      args.pop_back();
      args.pop_back();
      if (ofs.is_open())
      {
        ofs.close();
      }
      ofs.open(name);
      ofs.precision(10);
      return &ofs;
    }
  }
  std::cout.precision(10);
  return &std::cout;
}

void Command::clearStream()
{
  if (ofs.is_open())
  {
    ofs.close();
    ofs.close();
  }
}

void Command::die()
{
  dead = true;
}

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if (ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

int Command::waitChar()
{
  while (!kbhit() && !dead)
  {
    usleep(1000);
  }

  if (dead)
  {
    return 1024;
  }

  return getchar();
}
}  // namespace RhIO
