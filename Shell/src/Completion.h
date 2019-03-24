#pragma once

#include <string>
#include <deque>
#include <vector>
#include <sstream>

namespace RhIO
{
class Completion
{
public:
  static std::string getSubstring(std::deque<std::string> matches);
  static std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems);
  static std::vector<std::string> split(const std::string& s, char delim);
};
}  // namespace RhIO
