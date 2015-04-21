#pragma once

#include <string>

namespace RhIO
{
    class Terminal
    {
        public:
            /**
             * Handle format of the terminal
             */
            static void clear();
            static void setColor(std::string name, bool bold=false);
    };
}
