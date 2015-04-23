#pragma once

#include <string>
#include <deque>

namespace RhIO
{
    class Completion
    {
        public:
        static std::string getSubstring(std::deque<std::string> matches);
    };
}
