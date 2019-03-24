#include "Completion.h"
#include <iostream>

namespace RhIO
{
// probably a very unefficient way to do that
std::string Completion::getSubstring(std::deque<std::string> matches)
{
  std::string line = "";
  // so there are at least two elements but lets check anyway...
  if (matches.size() > 1)
  {
    size_t len = 9999;
    std::string tmpcmd;
    for (std::deque<std::string>::iterator it = matches.begin(); it != matches.end(); ++it)
    {
      if ((*it).length() < len)  // get the smallest
      {
        len = (*it).length();
        tmpcmd = *it;
      }
    }

    // ckeck char by char...
    for (size_t i = 1; i < tmpcmd.length() + 1; i++)  //+1?
    {
      bool iscommon = true;
      std::string tmp(tmpcmd, 0, i);

      for (std::deque<std::string>::iterator it = matches.begin(); it != matches.end(); ++it)
      {
        if ((*it).compare(tmpcmd) != 0)  // dont compare with ourself
        {
          if ((*it).compare(0, i, tmp) != 0)
            iscommon = false;
        }
      }
      if (iscommon)
        line = tmp;
    }
  }

  return line;
}

std::vector<std::string>& Completion::split(const std::string& s, char delim, std::vector<std::string>& elems)
{
  std::stringstream ss(s);
  std::string item;
  if (s.size() > 0)
  {
    while (std::getline(ss, item, delim))
    {
      elems.push_back(item);
    }
  }
  return elems;
}

std::vector<std::string> Completion::split(const std::string& s, char delim)
{
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}
}  // namespace RhIO
